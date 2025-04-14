#include <time.h>
#include "gestor_recursos/gestor_recursos.h"
#include "gestor_recursos/gestor_recursos_share.h"
#include <TAD/TAD.h>
#include <cspecs/cspec.h>
#include "t_config/t_config.h"

void wait_recursos_mutex(pthread_mutex_t mutex_recursos, t_dictionary *recursos, int from_size, int to_size)
{
     
    clock_t time_start, time_end;
    int time_past_sec;

    int recursos_size = from_size;
    time_start = clock();
    while(recursos_size != to_size) {
        pthread_mutex_lock(&mutex_recursos);
        recursos_size = dictionary_size(recursos);
        time_end = clock();
        time_past_sec = (time_end - time_start) / CLOCKS_PER_SEC;
        pthread_mutex_unlock(&mutex_recursos);
        if(time_past_sec >= 2)
        {
            printf("Timed Out %d", time_past_sec);
            break;
        }
    }
}

void wait_recurso_mutex(t_recurso *recurso, int from_size, int to_size)
{
     
    clock_t time_start, time_end;
    int time_past_sec;

    int solicitudes_size = from_size;
    time_start = clock();

    while(solicitudes_size != to_size) {
        pthread_mutex_lock(&recurso->mutex_recurso);
        solicitudes_size = list_size(recurso->solicitudes);
        time_end = clock();
        time_past_sec = (time_end - time_start) / CLOCKS_PER_SEC;
        pthread_mutex_unlock(&recurso->mutex_recurso);
        if(time_past_sec >= 2)
        {
            printf("Timed Out %d", time_past_sec);
            break;
        }
    }
}

context(Kernel_Gestor_Recursos) {

    describe("Gestor de recursos"){
    
        initialize_cfg("tests/testing.config");
        init_gestor_recursos();
        gestor_recursos_statics statics = get_gestor_recursos_statics();
        t_dictionary *recursos = *statics.recursos;
        pthread_mutex_t mutex_recursos = statics.mutex_recursos;

        it("debería poder crear y destruir recursos"){   

            agregar_recurso("systemA", 3, SISTEMA_SIGNAL_WAIT, -1);
            agregar_recurso("systemB", 3, SISTEMA_SIGNAL_WAIT, -1);
            agregar_recurso("systemC", 3, SISTEMA_SIGNAL_WAIT, -1);
            agregar_recurso("stdin", 1, IO_STDIN, -1);

            should_bool(dictionary_has_key(recursos, "systemA")) be equal to(true);
            should_bool(dictionary_has_key(recursos, "systemB")) be equal to(true);
            should_bool(dictionary_has_key(recursos, "systemC")) be equal to(true);
            should_bool(dictionary_has_key(recursos, "stdin")) be equal to(true);
            should_bool(dictionary_has_key(recursos, "RA")) be equal to(true); // Cargados de las config
            should_bool(dictionary_has_key(recursos, "RB")) be equal to(true); // Cargados de las config
            should_bool(dictionary_has_key(recursos, "RC")) be equal to(true); // Cargados de las config
            
            eliminar_recurso("systemB");
            eliminar_recurso("systemC");
            // eliminar_recurso("RA");
            eliminar_recurso("RB");
            eliminar_recurso("RC");

            // wait_recursos_mutex(mutex_recursos, recursos, 7, 3);

            // should_bool(dictionary_has_key(recursos, "systemA")) be equal to(true);
            // should_bool(dictionary_has_key(recursos, "stdin")) be equal to(true);
            
            // should_bool(dictionary_has_key(recursos, "systemB")) be equal to(false);
            // should_bool(dictionary_has_key(recursos, "systemC")) be equal to(false);
            // // should_bool(dictionary_has_key(recursos, "RA")) be equal to(false);
            // should_bool(dictionary_has_key(recursos, "RB")) be equal to(false);
            // should_bool(dictionary_has_key(recursos, "RC")) be equal to(false);

        } end

        it("deberia encolar y desencolar solicitudes"){
            uint32_t pid = 1;
            uint32_t real_mem_dir = 0; 
            uint32_t size = 3;
            s_kernel_io_stdin_read kernelio;
            t_paquete * kernelio_paquete;
            kernelio = create_kernel_io_stdin_read(pid, list_create(), size);
            kernelio_paquete = serializar_kernel_io_stdin_read(kernelio);
            t_instruccion instruccion = kernelio_paquete->codigo_operacion;
            
            t_recurso * recurso_stdin = dictionary_get(recursos, "stdin");

            encolar_solicitud("stdin", instruccion, (s_kernel_data) kernelio);
            wait_recurso_mutex(recurso_stdin, 0, 1);

            should_int(recurso_stdin->solicitudes->elements_count) be equal to(1);
            should_int(recurso_stdin->instancias_disponibles) be equal to(0);

            t_solicitud * stdin_solicitud_1 = (t_solicitud*) list_get(recurso_stdin->solicitudes, 0);

            should_int(stdin_solicitud_1->pid) be equal to(pid);
            should_bool(stdin_solicitud_1->procesando) be equal to(true);
            should_int(stdin_solicitud_1->tipo_instruccion) be equal to(instruccion);


            uint32_t pid_2 = 2; 
            uint32_t real_mem_dir_2 = 0; 
            uint32_t size_2 = 3;
            s_kernel_io_stdin_read kernelio_2;
            t_paquete * kernelio_paquete_2;
            kernelio_2 = create_kernel_io_stdin_read(pid_2, list_create(), size);
            kernelio_paquete_2 = serializar_kernel_io_stdin_read(kernelio_2);
            t_instruccion instruccion_2 = kernelio_paquete_2->codigo_operacion;

            encolar_solicitud("stdin", instruccion_2, (s_kernel_data) kernelio_2);
            wait_recurso_mutex(recurso_stdin, 1, 2);

            should_int(recurso_stdin->solicitudes->elements_count) be equal to(2);
            should_int(recurso_stdin->instancias_disponibles) be equal to(0);

            t_solicitud * stdin_solicitud_2 = (t_solicitud*)list_get(recurso_stdin->solicitudes, 1);

            should_int(stdin_solicitud_2->pid) be equal to(pid_2);
            should_bool(stdin_solicitud_2->procesando) be equal to(false);
            should_int(stdin_solicitud_2->tipo_instruccion) be equal to(instruccion_2);

            desencolar_solicitud_io("stdin", pid);
            wait_recurso_mutex(recurso_stdin, 2, 1);

            should_int(recurso_stdin->solicitudes->elements_count) be equal to(1);
            should_int(recurso_stdin->instancias_disponibles) be equal to(0);

            stdin_solicitud_2 = (t_solicitud*)list_get(recurso_stdin->solicitudes, 0);

            should_int(stdin_solicitud_2->pid) be equal to(pid_2);
            should_bool(stdin_solicitud_2->procesando) be equal to(true);
            should_int(stdin_solicitud_2->tipo_instruccion) be equal to(instruccion_2);

            desencolar_solicitud_io("stdin", pid_2);
            wait_recurso_mutex(recurso_stdin, 1, 0);

            should_int(recurso_stdin->solicitudes->elements_count) be equal to(0);
            should_int(recurso_stdin->instancias_disponibles) be equal to(1);

            paquete_destroy(kernelio_paquete);
            paquete_destroy(kernelio_paquete_2);

        } end

        it("deberia encolar y desencolar solicitudes de sistema"){
            uint32_t pid = 1;
            t_contexto_ejecucion ctx; 
            s_cpu_kernel s_cpu_kernel_wait = create_s_cpu_kernel_wait(ctx, "RA");
            serializar_cpu_kernel_wait(s_cpu_kernel_wait);

            s_cpu_kernel wait_o_signal;
            wait_o_signal.solicitud_recurso.args_signal = "RA";
            wait_o_signal.contexto.pid = pid;



            t_instruccion instruccion = CPU_KERNEL_DESALOJO_WAIT;
            
            t_recurso * recurso_sistema = dictionary_get(recursos, "RA");

            encolar_solicitud("RA", instruccion, (s_kernel_data) wait_o_signal);
            wait_recurso_mutex(recurso_sistema, 0, 1);

            should_int(recurso_sistema->solicitudes->elements_count) be equal to(1);
            should_int(recurso_sistema->instancias_disponibles) be equal to(0);

            t_solicitud * wait_solicitud_1 = (t_solicitud*) list_get(recurso_sistema->solicitudes, 0);

            should_int(wait_solicitud_1->pid) be equal to(pid);
            should_bool(wait_solicitud_1->procesando) be equal to(true);
            should_int(wait_solicitud_1->tipo_instruccion) be equal to(instruccion);


            uint32_t pid2 = 2;
            wait_o_signal.contexto.pid = pid2;
            t_instruccion instruccion2 = CPU_KERNEL_DESALOJO_WAIT;
            
            t_recurso * recurso_sistema2 = dictionary_get(recursos, "RA");

            encolar_solicitud("RA", instruccion2, (s_kernel_data) wait_o_signal);
            wait_recurso_mutex(recurso_sistema2, 1, 2);



            should_int(recurso_sistema2->solicitudes->elements_count) be equal to(2);
            should_int(recurso_sistema2->instancias_disponibles) be equal to(0);

            t_solicitud * wait_solicitud_2 = (t_solicitud*)list_get(recurso_sistema2->solicitudes, 1);

            should_int(wait_solicitud_2->pid) be equal to(pid2);
            should_bool(wait_solicitud_2->procesando) be equal to(false);
            should_int(wait_solicitud_2->tipo_instruccion) be equal to(instruccion2);

            desencolar_solicitud_sistema("RA", pid, false);
            wait_recurso_mutex(recurso_sistema2, 2, 1);

            should_int(recurso_sistema2->solicitudes->elements_count) be equal to(1);
            should_int(recurso_sistema2->instancias_disponibles) be equal to(0);

            wait_solicitud_2 = (t_solicitud*)list_get(recurso_sistema2->solicitudes, 0);

            should_int(wait_solicitud_2->pid) be equal to(pid2);
            should_bool(wait_solicitud_2->procesando) be equal to(true);
            should_int(wait_solicitud_2->tipo_instruccion) be equal to(instruccion);

            desencolar_solicitud_sistema("RA", pid2, false);
            wait_recurso_mutex(recurso_sistema2, 1, 0);

            should_int(recurso_sistema2->solicitudes->elements_count) be equal to(0);
            should_int(recurso_sistema2->instancias_disponibles) be equal to(1);

        } end
        
        destroy_gestor_recursos();
        recursos = NULL;
        // statics.destroy_solicitud(stdin_solicitud_1);
        // stdin_solicitud_1 = NULL;
        // statics.destroy_solicitud(stdin_solicitud_2);
        // stdin_solicitud_2 = NULL;
        destroy_config_kernel(get_config());

    } end

}
