#include "comunicacion_entradasalida/io_stdin/io_stdin.h"
#include "gestor_recursos/gestor_recursos_share.h"
#include "gestor_recursos/gestor_recursos.h"
#include "t_config/t_config.h"
#include <cspecs/cspec.h>

extern s_send_kernel_io_stdin_read_args send_kernel_io_stdin_read_args;
extern int recv_kernel_io_stdin_read_args;
extern s_planificador_corto_args args_p_corto_io_stdin_test;

context(comunicacion_entradasalida_io_stdin) {
    initialize_cfg("tests/testing.config");
    init_gestor_recursos();
    gestor_recursos_statics ges_rec = get_gestor_recursos_statics();
    t_dictionary *recursos = *ges_rec.recursos;

    char * nombre_io = "io_stdin";
    int instancias = 1;
    int fd_io_stdin = 3;
    agregar_recurso(nombre_io, instancias, IO_STDIN, fd_io_stdin);
    t_recurso * recurso = dictionary_get(recursos, nombre_io);

    uint32_t pid = 4;
    uint32_t real_mem_dir = 10;
    uint32_t size = 20;
    s_kernel_io_stdin_read io_stdin = create_kernel_io_stdin_read(pid, list_create(), size);
    t_paquete * kernelio_paquete = serializar_kernel_io_stdin_read(io_stdin);
    t_instruccion instruccion = (t_instruccion) kernelio_paquete->codigo_operacion;
    s_kernel_data kernel_data;
    kernel_data.stdin_read = io_stdin;
    encolar_solicitud(nombre_io, instruccion, kernel_data);
    t_solicitud *solicitud = (t_solicitud *) list_get(recurso->solicitudes, 0);

    describe("handle_io_stdin_read"){
        
        it("Debería hacer send, recieve y llamar al planificador corto"){            
            args_p_solicitud *args = malloc(sizeof(args_p_solicitud));
            args->solicitud = solicitud;
            args->recurso = recurso;
            
            handle_io_stdin_read(solicitud, recurso);

            // send a la io
            should_int((int)send_kernel_io_stdin_read_args.msj.pid) be equal to((int)pid);

            // recieve a la io
            should_int(recv_kernel_io_stdin_read_args) be equal to(fd_io_stdin);
            
            // planificador corto
            should_int(args_p_corto_io_stdin_test.transicion) be equal to(BLOCKED_A_READY);
            should_int(args_p_corto_io_stdin_test.op_pid.PID) be equal to((int)pid);
        }end
    }end

    destroy_gestor_recursos();
}
