#include "mensajes_a_memoria.h"

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void send_obtener_instruccion(uint32_t pid, uint32_t program_counter)
{
    s_memoria_cpu_instruccion s_mem_cpu_instruccion = create_mem_cpu_instruccion_struct(pid, program_counter);
    t_paquete *paquete = serializar_c_m_instruccion(s_mem_cpu_instruccion);
    send_paquete(get_fd_memoria(), paquete);
}

char *recv_instruccion()
{
    t_log *logger = get_logger();
    t_paquete *paquete_a_recibir;
    recv_paquete(get_fd_memoria(), &paquete_a_recibir);
    if (paquete_a_recibir->codigo_operacion == MEMORIA_CPU_OBTENER_INSTRUCCIONES)
    {
        log_info(logger, "Recibida la instruccion enviada por memoria.\n");
        char *instruccion;
        deserializar_string(paquete_a_recibir->buffer, &instruccion);
        paquete_destroy(paquete_a_recibir);
        return instruccion;
    }
    log_error(logger, "No se recibio la instruccion.\n");
    paquete_destroy(paquete_a_recibir);
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(1);
}

char *obtener_instruccion(uint32_t pid, uint32_t program_counter)
{
    send_obtener_instruccion(pid, program_counter);
    return recv_instruccion();
}

void obtener_tam_pag()
{
    t_log *logger = get_logger();
    t_paquete *paquete = paquete_new();
    t_paquete *paquete_a_recibir;
    uint32_t tam_pag;

    paquete->codigo_operacion = CPU_MEMORIA_OBTENER_TAM_PAG;
    send_paquete(get_fd_memoria(), paquete);

    recv_paquete(get_fd_memoria(), &paquete_a_recibir);
    if (paquete_a_recibir->codigo_operacion == MEMORIA_CPU_TAM_PAG)
    {
        deserializar_uint32_t(paquete_a_recibir->buffer, &tam_pag);

        // NOLINTNEXTLINE(clang-diagnostic-format-security)
        log_info(logger, "El tamanio de pagina es %i.\n", tam_pag);
        paquete_destroy(paquete_a_recibir);

        set_tam_pag(tam_pag);
    }
    else
    {
        log_error(logger, "No se recibio el tamanio de la pagina.\n");
        paquete_destroy(paquete_a_recibir);
        // NOLINTNEXTLINE(concurrency-mt-unsafe)
        exit(1);
    }
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void send_resize(uint32_t pid, uint32_t tamanio)
{
    s_memoria_cpu_resize s_mem_cpu_resize = create_mem_cpu_resize_struct(pid, tamanio);
    t_paquete *paquete = serializar_c_m_resize(s_mem_cpu_resize);
    paquete->codigo_operacion = CPU_MEMORIA_RESIZE;
    send_paquete(get_fd_memoria(), paquete);
}

void send_mov_in(uint32_t addres, uint32_t size)
{
    s_cpu_mem_movin s_m_c_movin = create_cpu_mem_movin(get_pid(), addres, size);
    t_paquete *paquete = serializar_c_m_movin(s_m_c_movin);
    paquete->codigo_operacion = CPU_MEMORIA_MOV_IN;
    send_paquete(get_fd_memoria(), paquete);
}

uint8_t recv_mov_in()
{
    t_paquete *paquete_a_recibir;
    recv_paquete(get_fd_memoria(), &paquete_a_recibir);
    assert(paquete_a_recibir->codigo_operacion == MEMORIA_CPU_MOV_IN);
    uint8_t bytes_recibidos;
    deserializar_uint8_t(paquete_a_recibir->buffer, &bytes_recibidos);
    return bytes_recibidos;
}

void send_mov_out(uint32_t address, uint32_t chunk_size, uint8_t data)
{
    s_cpu_mem_movout s_c_m_movout = create_cpu_mem_movout(get_pid(), address, chunk_size, data);
    t_paquete *pkg = serializar_c_m_movout(s_c_m_movout);
    send_paquete(get_fd_memoria(), pkg);
}

bool recv_resize()
{
    t_log *logger = get_logger();
    t_paquete *paquete_a_recibir;
    recv_paquete(get_fd_memoria(), &paquete_a_recibir);
    if (paquete_a_recibir->codigo_operacion == OK_GRAL)
    {
        log_info(logger, "Se cambio el tamanio del proceso con exito.\n");
        return true;
    }
    if (paquete_a_recibir->codigo_operacion == MEMORIA_CPU_OUT_OF_MEMORY)
    {
        log_info(logger, "Recibi un OUT OF MEMORY\n");
        return false;
    }
    log_error(logger, "No se recibio la instruccion.\n");
    paquete_destroy(paquete_a_recibir);
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(1);
}

static uint32_t recv_frame()
{
    t_paquete *paquete_a_recibir;
    recv_paquete(get_fd_memoria(), &paquete_a_recibir);
    if (paquete_a_recibir->codigo_operacion == MEMORIA_CPU_OBTENER_MARCO)
    {
        uint32_t frame;
        deserializar_uint32_t(paquete_a_recibir->buffer, &frame);
        return frame;
    }
    log_error(get_logger(), "No se recibio el marco.\n");
    paquete_destroy(paquete_a_recibir);
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(1);
}

uint32_t obtener_frame(uint32_t pid, uint32_t pagina)
{
    s_cpu_memoria_marco s_cpu_mem_marco = {.pid = pid, .pag = pagina};
    t_paquete *paquete = serializar_c_m_marco(s_cpu_mem_marco);
    send_paquete(get_fd_memoria(), paquete);

    uint32_t frame = recv_frame();
    log_info(get_logger(), "PID: %i - OBTENER MARCO - Página: %i - Marco: %i\n", get_pid(), pagina, frame);
    return frame;
}

bool send_copy_string(uint32_t pid, t_list *source, t_list *destiny, uint32_t size)
{
    s_cpu_memoria_copy_string cpy_str = {.pid = pid, .source = source, .destiny = destiny, .size = size};
    t_paquete *pkt = serializar_c_m_copy_string(cpy_str);
    bool status = send_paquete(get_fd_memoria(), pkt);

    // recv_paquete(get_fd_memoria(), &paquete_a_recibir); TODO(all): no nos importa la respuesta ?

    if (status == false)
    {
        return false;
    }
    return true;
}