#include "comunicacion_entradasalida/io_fs/io_fs.h"
#include "gestor_recursos/gestor_recursos_share.h"
#include "gestor_recursos/gestor_recursos.h"
#include "t_config/t_config.h"
#include <utils/console.h>
#include <cspecs/cspec.h>

extern s_send_kernel_io_fs_args send_kernel_io_fs_args;
extern int recv_io_kernel_fs_args;
extern s_planificador_corto_args args_p_corto_io_fs_test;

context(comunicacion_entradasalida_io_fs) {
    initialize_cfg("tests/testing.config");

    consola_con_censura();

    init_gestor_recursos();
    gestor_recursos_statics ges_rec = get_gestor_recursos_statics();
    t_dictionary *recursos = *ges_rec.recursos;

    char * nombre_io = "io_fs";
    int instancias = 1;
    int fd_io_fs = 100;
    agregar_recurso(nombre_io, instancias, IO_FS, fd_io_fs);
    t_recurso * recurso = dictionary_get(recursos, nombre_io);

    uint32_t pid = 8;
    char * file_name = "file_name";
    uint32_t size = 10;
    uint32_t file_pointer = 20;
    uint32_t real_mem_dir = 200;

    s_kernel_data kernel_data;
    
    s_kernel_io_fs_create fs_create = create_kernel_io_fs_create(pid, file_name);
    kernel_data.fs_create = fs_create;
    t_paquete * paquete_fs_create = serializar_kernel_io_fs_create(fs_create);
    t_instruccion instruccion_fs_create = (t_instruccion) paquete_fs_create->codigo_operacion;
    encolar_solicitud(nombre_io, instruccion_fs_create, kernel_data);
    t_solicitud *solicitud_fs_create = (t_solicitud *) list_get(recurso->solicitudes, 0);

    s_kernel_io_fs_delete fs_delete = create_kernel_io_fs_delete(pid, file_name);
    kernel_data.fs_delete = fs_delete;
    t_paquete * paquete_fs_delete = serializar_kernel_io_fs_delete(fs_delete);
    t_instruccion instruccion_fs_delete = (t_instruccion) paquete_fs_delete->codigo_operacion;
    encolar_solicitud(nombre_io, instruccion_fs_delete, kernel_data);
    t_solicitud *solicitud_fs_delete = (t_solicitud *) list_get(recurso->solicitudes, 1);


    // Creo el info marcos
    t_list* info_marcos = list_create();
    //-----ENTRADA 1-----
    frames_element *frames_element_pkt1 = malloc(sizeof(frames_element));
    frames_element_pkt1->pid = 7;
    frames_element_pkt1->offset = 8;
    frames_element_pkt1->padding = 8;
    frames_element_pkt1->frame = 1;
    frames_element_pkt1->tam_pag = 16;
    list_add(info_marcos, (void *)frames_element_pkt1);
    //-----ENTRADA 2-----
    frames_element *frames_element_pkt2 = malloc(sizeof(frames_element));
    frames_element_pkt2->pid = 7;
    frames_element_pkt2->offset = 0;
    frames_element_pkt2->padding = 16;
    frames_element_pkt2->frame = 2;
    frames_element_pkt2->tam_pag = 16;
    list_add(info_marcos, (void *)frames_element_pkt2);

    s_kernel_io_fs_read fs_read = create_kernel_io_fs_read(pid, file_name, file_pointer, info_marcos, size);
    kernel_data.fs_read = fs_read;
    t_paquete * paquete_fs_read = serializar_kernel_io_fs_read(fs_read);
    t_instruccion instruccion_fs_read = (t_instruccion) paquete_fs_read->codigo_operacion;
    encolar_solicitud(nombre_io, instruccion_fs_read, kernel_data);
    t_solicitud *solicitud_fs_read = (t_solicitud *) list_get(recurso->solicitudes, 2);

    s_kernel_io_fs_truncate fs_truncate = create_kernel_io_fs_truncate(pid, file_name, size);
    kernel_data.fs_truncate = fs_truncate;
    t_paquete * paquete_fs_truncate = serializar_kernel_io_fs_truncate(fs_truncate);
    t_instruccion instruccion_fs_truncate = (t_instruccion) paquete_fs_truncate->codigo_operacion;
    encolar_solicitud(nombre_io, instruccion_fs_truncate, kernel_data);
    t_solicitud *solicitud_fs_truncate = (t_solicitud *) list_get(recurso->solicitudes, 3);

    s_kernel_io_fs_write fs_write = create_kernel_io_fs_write(pid, file_name, file_pointer, info_marcos, size);
    kernel_data.fs_write = fs_write;
    t_paquete * paquete_fs_write = serializar_kernel_io_fs_write(fs_write);
    t_instruccion instruccion_fs_write = (t_instruccion) paquete_fs_write->codigo_operacion;
    encolar_solicitud(nombre_io, instruccion_fs_write, kernel_data);
    t_solicitud *solicitud_fs_write = (t_solicitud *) list_get(recurso->solicitudes, 4);

    consola_con_libre_expresion();

    describe("handle_io_fs_create"){
        
        it("Debería hacer send, recieve y llamar al planificador corto"){
            
            consola_con_censura();
            handle_io_fs_create(solicitud_fs_create, recurso);
            consola_con_libre_expresion();

            // send a la io
            should_int((int)send_kernel_io_fs_args.msj.fs_create.pid) be equal to((int)pid);
            should_string(send_kernel_io_fs_args.msj.fs_create.file_name) be equal to(file_name);

            // recieve a la io
            should_int(recv_io_kernel_fs_args) be equal to(fd_io_fs);
            
            // planificador corto
            should_int(args_p_corto_io_fs_test.transicion) be equal to(BLOCKED_A_READY);
            should_int(args_p_corto_io_fs_test.op_pid.PID) be equal to((int)pid);
        }end
    }end

    describe("handle_io_fs_delete"){
        it("Debería hacer send, recieve y llamar al planificador corto"){            
            
            consola_con_censura();
            handle_io_fs_delete(solicitud_fs_delete, recurso);
            consola_con_libre_expresion();

            // send a la io
            should_int((int)send_kernel_io_fs_args.msj.fs_delete.pid) be equal to((int)pid);
            should_string(send_kernel_io_fs_args.msj.fs_delete.file_name) be equal to(file_name);

            // recieve a la io
            should_int(recv_io_kernel_fs_args) be equal to(fd_io_fs);
            
            // planificador corto
            should_int(args_p_corto_io_fs_test.transicion) be equal to(BLOCKED_A_READY);
            should_int(args_p_corto_io_fs_test.op_pid.PID) be equal to((int)pid);
        }end
    }end

    describe("handle_io_fs_read"){
        it("Debería hacer send, recieve y llamar al planificador corto"){

            consola_con_censura();
            handle_io_fs_read(solicitud_fs_read, recurso);
            consola_con_libre_expresion();


            // send a la io
            should_int((int)send_kernel_io_fs_args.msj.fs_read.pid) be equal to((int)pid);
            should_string(send_kernel_io_fs_args.msj.fs_read.file_name) be equal to(file_name);
            should_int(send_kernel_io_fs_args.msj.fs_read.file_pointer) be equal to(file_pointer);

            frames_element *resultado = list_get(send_kernel_io_fs_args.msj.fs_read.info_marcos, 0);
            //Primer frame
            should_int(resultado->pid) be equal to(7);
            should_int(resultado->frame) be equal to(1);
            //Segundo frame
            resultado = list_get(send_kernel_io_fs_args.msj.fs_read.info_marcos, 1);
            should_int(resultado->pid) be equal to(7);
            should_int(resultado->frame) be equal to(2);

            should_int(send_kernel_io_fs_args.msj.fs_read.size) be equal to(size);

            // recieve a la io
            should_int(recv_io_kernel_fs_args) be equal to(fd_io_fs);
            
            // planificador corto
            should_int(args_p_corto_io_fs_test.transicion) be equal to(BLOCKED_A_READY);
            should_int(args_p_corto_io_fs_test.op_pid.PID) be equal to((int)pid);
        }end
    }end

    describe("handle_io_fs_truncate"){
        it("Debería hacer send, recieve y llamar al planificador corto"){            
            
            consola_con_censura();
            handle_io_fs_truncate(solicitud_fs_truncate, recurso);
            consola_con_libre_expresion();

            // send a la io
            should_int((int)send_kernel_io_fs_args.msj.fs_truncate.pid) be equal to((int)pid);
            should_string(send_kernel_io_fs_args.msj.fs_truncate.file_name) be equal to(file_name);            should_int(send_kernel_io_fs_args.msj.fs_read.size) be equal to(size);

            // recieve a la io
            should_int(recv_io_kernel_fs_args) be equal to(fd_io_fs);
            
            // planificador corto
            should_int(args_p_corto_io_fs_test.transicion) be equal to(BLOCKED_A_READY);
            should_int(args_p_corto_io_fs_test.op_pid.PID) be equal to((int)pid);
        }end
    }end

    describe("handle_io_fs_write"){
        it("Debería hacer send, recieve y llamar al planificador corto"){            

            consola_con_censura();
            handle_io_fs_write(solicitud_fs_write, recurso);
            consola_con_libre_expresion();
            
            // send a la io
            should_int((int)send_kernel_io_fs_args.msj.fs_write.pid) be equal to((int)pid);
            should_string(send_kernel_io_fs_args.msj.fs_write.file_name) be equal to(file_name);            should_int(send_kernel_io_fs_args.msj.fs_read.size) be equal to(size);

            // recieve a la io
            should_int(recv_io_kernel_fs_args) be equal to(fd_io_fs);
            
            // planificador corto
            should_int(args_p_corto_io_fs_test.transicion) be equal to(BLOCKED_A_READY);
            should_int(args_p_corto_io_fs_test.op_pid.PID) be equal to((int)pid);
        }end
    }end

    destroy_gestor_recursos();
}
