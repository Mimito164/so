#include "iniciar_servidor_memoria.h"

// Kernel crea un proceso y lo manda a memoria sin esperar respuesta
// Crear un proceso tiene que leer un archivo y eso es IO (lento)
// Necesitamos terminar con la operación de crear el proceso
// Y luego podemos darle las instrucciones a cpu de ese proceso
static pthread_mutex_t kernel_instructions;
static bool kernel_m_init = false;

static void init_kernel_mutex()
{
    if (kernel_m_init)
    {
        return;
    }
    kernel_m_init = true;
    pthread_mutex_init(&kernel_instructions, NULL);
}

void print_pkt(void *raw_pkt)
{
    frames_element *pkt = (frames_element *)raw_pkt;
    printf("pkt \n");
    printf("pid %i \n", pkt->pid);
    printf("frame %i \n", pkt->frame);
    printf("tam_pag %i \n", pkt->tam_pag);
    printf("offset %i \n", pkt->offset);
    printf("padding %i \n", pkt->padding);
}

void atender_cliente(void *args)
{

    t_log *logger = get_logger();
    client_thread_args *thread_args = (client_thread_args *)args;
    t_paquete *paquete_a_recibir;
    const int mil = 1000;

    while (recv_paquete(*(thread_args->fd), &paquete_a_recibir))
    {

        // aca se manejan todas las peticiones que puede llegar a pedir un cliente
        switch (paquete_a_recibir->codigo_operacion)
        {

        case KERNEL_MEMORIA_CREAR_PROCESO: {
            log_debug(logger, "Recibido opcode: KERNEL_MEMORIA_CREAR_PROCESO");

            pthread_mutex_lock(&kernel_instructions);

            s_kernel_memoria_creacion_proceso k_m_creacion_pros = deserializar_k_m_creacion_proceso(paquete_a_recibir);
            log_debug(logger, "pre agregar_instrucciones_en_tabla");
            t_list *lista_instrucciones = cargar_instrucciones(k_m_creacion_pros.path);
            log_debug(logger, "pre2 agregar_instrucciones_en_tabla");
            agregar_instrucciones_en_tabla(k_m_creacion_pros.pid, lista_instrucciones);
            agregar_tabla_paginas_proceso_en_tabla_global(k_m_creacion_pros.pid);

            pthread_mutex_unlock(&kernel_instructions);

            break;
        }
        case KERNEL_MEMORIA_FINALIZAR_PROCESO: {
            log_debug(logger, "Recibido opcode: KERNEL_MEMORIA_FINALIZAR_PROCESO\n");

            // No tenemos que bloquear a nadie para matar un proceso, no es prioritario
            // Incluso CPU puede pedirnos una instruccion de un proceso que kernel acaba de matar
            // Hasta podríamos demorarnos en matar un proceso

            uint32_t pid_a_finalizar = deserializar_k_m_finalizacion_proceso(paquete_a_recibir);
            t_tabla_paginas_proceso *tabla = quitar_tabla_paginas_proceso(pid_a_finalizar);
            sem_wait(&(tabla->mutex));
            log_info(get_logger(), "Destruccion de Tabla De Paginas PID %i - Tamanio %i", pid_a_finalizar,
                     list_size(tabla->frames));
            liberar_frames(list_size(tabla->frames), tabla->frames);
            destroy_tabla_paginas_proceso(tabla);
            destroy_tabla_instrucciones_proceso(pid_a_finalizar);

            break;
        }

        case CPU_MEMORIA_OBTENER_TAM_PAG: {
            pthread_mutex_lock(&kernel_instructions);
            pthread_mutex_unlock(&kernel_instructions);
            log_debug(logger, "Recibido opcode: CPU_MEMORIA_OBTENER_TAM_PAG");

            // NOLINTNEXTLINE(concurrency-mt-unsafe)
            usleep(get_config()->RETARDO_RESPUESTA * mil);

            send_tam_pag(*(thread_args->fd));
            break;
        }

        case CPU_MEMORIA_RESIZE: {
            pthread_mutex_lock(&kernel_instructions);
            pthread_mutex_unlock(&kernel_instructions);
            log_debug(logger, "Recibido opcode: CPU_MEMORIA_RESIZE");

            // NOLINTNEXTLINE(concurrency-mt-unsafe)
            usleep(get_config()->RETARDO_RESPUESTA * mil);

            s_memoria_cpu_resize s_mem_cpu_resize = deserializar_m_c_resize_struct(paquete_a_recibir);
            bool respuesta = resize(s_mem_cpu_resize.pid, s_mem_cpu_resize.size);
            if (respuesta)
            {
                send_ok(*(thread_args->fd));
            }
            else
            {
                send_out_of_memory(*(thread_args->fd));
            }
            break;
        }

        case CPU_MEMORIA_OBTENER_MARCO: {
            pthread_mutex_lock(&kernel_instructions);
            pthread_mutex_unlock(&kernel_instructions);
            log_debug(logger, "Recibido opcode: CPU_MEMORIA_OBTENER_MARCO");

            // NOLINTNEXTLINE(concurrency-mt-unsafe)
            usleep(get_config()->RETARDO_RESPUESTA * mil);

            s_cpu_memoria_marco s_cpu_mem_marco = deserializar_c_m_marco(paquete_a_recibir);
            int frame = obtener_frame(s_cpu_mem_marco.pid, s_cpu_mem_marco.pag);
            send_frame(*(thread_args->fd), frame);
            break;
        }
        case CPU_MEMORIA_OBTENER_INSTRUCCIONES: {
            pthread_mutex_lock(&kernel_instructions);
            pthread_mutex_unlock(&kernel_instructions);
            log_debug(logger, "Recibido opcode: CPU_MEMORIA_OBTENER_INSTRUCCIONES");

            s_memoria_cpu_instruccion s_mem_cpu_instruccion = deserializar_m_c_instruccion_struct(paquete_a_recibir);
            char *instruccion_requerida =
                obtener_instruccion(s_mem_cpu_instruccion.pid, s_mem_cpu_instruccion.program_counter);
            log_debug(get_logger(), "instrucción a enviar: %s", instruccion_requerida);
            // NOLINTNEXTLINE(concurrency-mt-unsafe)
            usleep(get_config()->RETARDO_RESPUESTA * mil);

            log_debug(get_logger(), "instrucción a enviar2: %s", instruccion_requerida);
            send_instruccion(*(thread_args->fd), instruccion_requerida);
            free(instruccion_requerida);
            break;
        }

        case CPU_MEMORIA_COPIAR_STRING: {
            pthread_mutex_lock(&kernel_instructions);
            pthread_mutex_unlock(&kernel_instructions);
            log_info(logger, "Recibido opcode: CPU_MEMORIA_COPIAR_STRING");

            s_cpu_memoria_copy_string s_c_m_copy_string = deserializar_c_m_copy_string(paquete_a_recibir);

            char *data =
                get_val_from_usermem_iterator(s_c_m_copy_string.source, s_c_m_copy_string.size); // lo toma de memoria

            // TODO(all): char *data probablemente tengamos que freearlo

            set_val_into_usermem_iterator(s_c_m_copy_string.destiny, data); // lo guarda en memoria

            usleep(get_config()->RETARDO_RESPUESTA * mil);

            break;
        }

        case CPU_MEMORIA_MOV_IN: {
            pthread_mutex_lock(&kernel_instructions);
            pthread_mutex_unlock(&kernel_instructions);
            log_debug(logger, "Recibido opcode: CPU_MEMORIA_MOV_IN");

            print_user_mem();
            // NOLINTNEXTLINE(concurrency-mt-unsafe)
            usleep(get_config()->RETARDO_RESPUESTA * mil);

            s_cpu_mem_movin s_c_m_movin = deserializar_c_m_movin(paquete_a_recibir);
            uint8_t bytes_to_send = *(char *)get_val_from_usermem(s_c_m_movin.address, s_c_m_movin.size);
            log_info(logger, "PID: <%d> - Accion: <LEER> - Direccion fisica: <%d> - Tamano <%d>", s_c_m_movin.pid,
                     s_c_m_movin.address, s_c_m_movin.size);
            send_bytes(*thread_args->fd, bytes_to_send);
            break;
        }

        case CPU_MEMORIA_MOV_OUT: {
            pthread_mutex_lock(&kernel_instructions);
            pthread_mutex_unlock(&kernel_instructions);
            log_debug(logger, "Recibido opcode: CPU_MEMORIA_MOV_OUT");

            print_user_mem();

            // NOLINTNEXTLINE(concurrency-mt-unsafe)
            usleep(get_config()->RETARDO_RESPUESTA * mil);
            s_cpu_mem_movout s_c_m_movout = deserializar_c_m_movout(paquete_a_recibir);
            void *data_casteada = &s_c_m_movout.data;
            set_val_to_usermem(data_casteada, s_c_m_movout.address, s_c_m_movout.size);
            log_info(logger, "PID: <%d> - Accion: <ESCRIBIR> - Direccion fisica: <%d> - Tamano <%d>", s_c_m_movout.pid,
                     s_c_m_movout.address, s_c_m_movout.size);

            send_ok(*thread_args->fd);
            break;
        }
        /**
         * IO_STDIN_READ (Interfaz, Registro Dirección, Registro Tamaño):
         * Esta instrucción solicita al Kernel que mediante la interfaz ingresada se lea
         * desde el STDIN (Teclado) un valor cuyo tamaño está delimitado por el valor
         * del Registro Tamaño y el mismo se guarde a partir de la Dirección Lógica
         * almacenada en el Registro Dirección.
         */
        case IO_MEMORIA_READ: { // se usaría para stdin y para fs (a priori)
            pthread_mutex_lock(&kernel_instructions);
            pthread_mutex_unlock(&kernel_instructions);
            log_debug(logger, "Recibido opcode: IO_MEMORIA_READ");

            // NOLINTNEXTLINE(concurrency-mt-unsafe)
            sleep((get_config()->RETARDO_RESPUESTA) / mil);
            s_io_memoria_read s_io_m_read = deserializar_io_memoria_read(paquete_a_recibir);
            set_val_to_usermem(s_io_m_read.data, s_io_m_read.real_mem_dir, s_io_m_read.size);
            print_user_mem();
            log_info(logger, "PID: <%d> - Accion: <ESCRIBIR> - Direccion fisica: <%d> - Tamano <%d>", s_io_m_read.pid,
                     s_io_m_read.real_mem_dir, s_io_m_read.size);
            send_ok(*thread_args->fd);

            break;
        }

        /**
         *
         * IO_STDOUT_WRITE (Interfaz, Registro Dirección, Registro Tamaño):
         * Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se lea
         *  desde la posición de memoria indicada por la Dirección Lógica almacenada en
         * el Registro Dirección, un tamaño indicado por el Registro Tamaño y se imprima por pantalla.
         *
         */
        case IO_MEMORIA_WRITE: { // se usaría para stdin y para fs (a priori)
            pthread_mutex_lock(&kernel_instructions);
            pthread_mutex_unlock(&kernel_instructions);
            log_debug(logger, "Recibido opcode: IO_MEMORIA_WRITE");

            print_user_mem();
            // NOLINTNEXTLINE(concurrency-mt-unsafe)
            sleep((get_config()->RETARDO_RESPUESTA) / mil);

            s_io_memoria_write s_io_m_write = deserializar_io_memoria_write(paquete_a_recibir);
            char *bytes_to_send = (char *)get_val_from_usermem(s_io_m_write.real_mem_dir, s_io_m_write.size);
            log_info(logger, "PID: <%d> - Accion: <LEER> - Direccion fisica: <%d> - Tamano <%d>", s_io_m_write.pid,
                     s_io_m_write.real_mem_dir, s_io_m_write.size);

            send_m_i_write(*thread_args->fd, s_io_m_write.real_mem_dir, s_io_m_write.size, bytes_to_send);

            break;
        }

        default:
            log_error(logger, "La memoria recibio cualquier cosa");
            break;
        }
        paquete_destroy(paquete_a_recibir);
    }
    free(thread_args->fd);
    free(thread_args);
    log_warning(logger, "Se desconecto un cliente");

    // paquete_destroy(paquete_a_recibir);
    // free(thread_args->fd);
    // free(thread_args);
}

void iniciar_servidor_memoria()
{
    t_log *logger = get_logger();
    t_config_memoria *cfg = get_config();
    int fd_escucha = iniciar_servidor(logger, "Servidor Memoria", cfg->IP_MODULO, cfg->PUERTO_ESCUCHA);
    init_kernel_mutex();

    while (1)
    {
        pthread_t thread;
        client_thread_args *args = malloc(sizeof(client_thread_args));
        args->fd = malloc(sizeof(int));
        *(args->fd) = esperar_cliente(logger, "Server Memoria", fd_escucha);
        pthread_create(&thread, NULL, (void *)atender_cliente, (void *)args);
        pthread_detach(thread);
    }
}
