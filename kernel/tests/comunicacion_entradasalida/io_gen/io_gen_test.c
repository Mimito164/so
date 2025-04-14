#include "comunicacion_entradasalida/io_gen/io_gen.h"
#include "gestor_recursos/gestor_recursos_share.h"
#include "gestor_recursos/gestor_recursos.h"
#include "t_config/t_config.h"
#include <cspecs/cspec.h>

extern s_send_kernel_io_gen_args send_kernel_io_gen_args;
extern int recv_io_kernel_gen_sleep_args;
extern s_planificador_corto_args args_p_corto_io_gen_test;

context(comunicacion_entradasalida_io_gen) {
    initialize_cfg("tests/testing.config");
    init_gestor_recursos();
    gestor_recursos_statics ges_rec = get_gestor_recursos_statics();
    t_dictionary *recursos = *ges_rec.recursos;

    char * nombre_io = "io_gen";
    int instancias = 1;
    int fd_io_gen = 1;
    agregar_recurso(nombre_io, instancias, IO_GEN, fd_io_gen);
    t_recurso * recurso = dictionary_get(recursos, nombre_io);

    uint32_t pid = 1;
    uint32_t work_unit = 3;
    s_kernel_io_sleep io_sleep = create_kernel_io_sleep_struct(pid, work_unit);
    t_paquete * kernelio_paquete = serializar_kernel_io_sleep_struct(io_sleep);
    t_instruccion instruccion = (t_instruccion) kernelio_paquete->codigo_operacion;
    s_kernel_data kernel_data;
    kernel_data.sleep = io_sleep;
    encolar_solicitud(nombre_io, instruccion, kernel_data);
    t_solicitud *solicitud = (t_solicitud *) list_get(recurso->solicitudes, 0);

    describe("handle_io_gen_sleep"){
        
        it("Debería hacer send, recieve y llamar al planificador corto"){            
            args_p_solicitud *args = malloc(sizeof(args_p_solicitud));
            args->solicitud = solicitud;
            args->recurso = recurso;
            
            handle_io_gen_sleep(solicitud, recurso);

            // send a la io
            should_int((int)send_kernel_io_gen_args.msj.pid) be equal to((int)pid);
            should_int((int)send_kernel_io_gen_args.msj.work_unit) be equal to((int)work_unit);

            // recieve a la io
            should_int(recv_io_kernel_gen_sleep_args) be equal to(fd_io_gen);
            
            // planificador corto
            should_int(args_p_corto_io_gen_test.transicion) be equal to(BLOCKED_A_READY);
            should_int(args_p_corto_io_gen_test.op_pid.PID) be equal to((int)pid);
        }end
    }end

    destroy_gestor_recursos();
}
