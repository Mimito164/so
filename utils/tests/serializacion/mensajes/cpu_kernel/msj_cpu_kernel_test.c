// #include "serializacion/mensajes/cpu_kernel/msj_cpu_kernel.h"
// #include "serializacion/serializacion.h"
// #include <cspecs/cspec.h>

// void check_contexto(t_contexto_ejecucion actual, t_contexto_ejecucion expected) {
//     should_int(actual.pid) be equal to(expected.pid);
//     should_int(actual.registros_cpu.PC) be equal to(expected.registros_cpu.PC);
//     should_int(actual.registros_cpu.AX) be equal to(expected.registros_cpu.AX);
//     should_int(actual.registros_cpu.BX) be equal to(expected.registros_cpu.BX);
//     should_int(actual.registros_cpu.CX) be equal to(expected.registros_cpu.CX);
//     should_int(actual.registros_cpu.DX) be equal to(expected.registros_cpu.DX);
//     should_int(actual.registros_cpu.EAX) be equal to(expected.registros_cpu.EAX);
//     should_int(actual.registros_cpu.EBX) be equal to(expected.registros_cpu.EBX);
//     should_int(actual.registros_cpu.ECX) be equal to(expected.registros_cpu.ECX);
//     should_int(actual.registros_cpu.EDX) be equal to(expected.registros_cpu.EDX);
//     should_int(actual.registros_cpu.SI) be equal to(expected.registros_cpu.SI);
//     should_int(actual.registros_cpu.DI) be equal to(expected.registros_cpu.DI);
// }

// void set_context(t_contexto_ejecucion* dummy_ctx) {
//     dummy_ctx->pid = 69;
//     dummy_ctx->registros_cpu.PC = 10;
//     dummy_ctx->registros_cpu.AX = 101;
//     dummy_ctx->registros_cpu.BX = 102;
//     dummy_ctx->registros_cpu.CX = 103;
//     dummy_ctx->registros_cpu.DX = 104;
//     dummy_ctx->registros_cpu.EAX = 201;
//     dummy_ctx->registros_cpu.EBX = 202;
//     dummy_ctx->registros_cpu.ECX = 203;
//     dummy_ctx->registros_cpu.EDX = 204;
//     dummy_ctx->registros_cpu.SI = 164;
//     dummy_ctx->registros_cpu.DI = 461;
// }

// context(msj_cpu_kernel) {

//     describe("CPU_KERNEL: Contexto de ejecucion"){

//         t_contexto_ejecucion dummy_ctx;
//         t_paquete * un_paquete;
//         before {
//             un_paquete = paquete_new();
//             set_context(&dummy_ctx);
//         }end

//         after {
//             paquete_destroy(un_paquete);
//         }end

//         it("debería ser serializado y deserializado correctamente"){
//             serializar_contexto_ejecucion(&un_paquete->buffer, dummy_ctx);
//             t_contexto_ejecucion contexto_recibido = deserializar_contexto_ejecucion(un_paquete);
//             check_contexto(contexto_recibido, dummy_ctx);
//         }end

//     }end

//     describe("Mensaje con op_code CPU_KERNEL_DESALOJO_SIGNAL") {
//         t_contexto_ejecucion dummy_ctx;
//         t_paquete * un_paquete;
//         s_cpu_kernel s_cpu_kernel;
//         before {
//             set_context(&dummy_ctx);
//         }end

//         after {
//         }end

//         it("debería ser serializado y deserializado correctamente"){

//             char* nombre_recurso = "boke";
//             s_cpu_kernel = create_s_cpu_kernel_signal(dummy_ctx, nombre_recurso);
//             un_paquete = serializar_cpu_kernel_signal(s_cpu_kernel);
//             should_int(un_paquete->codigo_operacion) be equal to(CPU_KERNEL_DESALOJO_SIGNAL);
//             s_cpu_kernel =  deserializar_cpu_kernel_signal(un_paquete);
//             check_contexto(s_cpu_kernel.contexto, dummy_ctx);
//             should_string(s_cpu_kernel.solicitud_recurso.args_signal) be equal to(nombre_recurso);
//         }end

//     }end

//     describe("Mensaje con op_code CPU_KERNEL_DESALOJO_WAIT") {
//         t_contexto_ejecucion dummy_ctx;
//         t_paquete * un_paquete;
//         s_cpu_kernel s_cpu_kernel;
//         before {
//             set_context(&dummy_ctx);
//         }end

//         after {
//         }end

//         it("debería ser serializado y deserializado correctamente"){

//             char* nombre_recurso = "boke";
//             s_cpu_kernel = create_s_cpu_kernel_wait(dummy_ctx, nombre_recurso);
//             un_paquete = serializar_cpu_kernel_wait(s_cpu_kernel);
//             should_int(un_paquete->codigo_operacion) be equal to(CPU_KERNEL_DESALOJO_WAIT);
//             s_cpu_kernel =  deserializar_cpu_kernel_wait(un_paquete);
//             check_contexto(s_cpu_kernel.contexto, dummy_ctx);
//             should_string(s_cpu_kernel.solicitud_recurso.args_wait) be equal to(nombre_recurso);
//         }end

//     }end

//     describe("Mensaje con op_code CPU_KERNEL_DESALOJO_IO_GEN_ZZZ") {
//         t_contexto_ejecucion dummy_ctx;
//         t_paquete * un_paquete;
//         s_cpu_kernel s_cpu_kernel;
//         before {
//             set_context(&dummy_ctx);
//         }end

//         after {
//         }end

//         it("debería ser serializado y deserializado correctamente"){

//             char* nombre_io = "palermo";
//             uint32_t unidades_trabajo = 11 + 1;

//             s_cpu_kernel = create_s_cpu_kernel_io_sleep(dummy_ctx, nombre_io,unidades_trabajo );

//             un_paquete = serializar_cpu_kernel_io_sleep(s_cpu_kernel);
//             should_int(un_paquete->codigo_operacion) be equal to(CPU_KERNEL_DESALOJO_IO_GEN_ZZZ);
//             s_cpu_kernel =  deserializar_cpu_kernel_io_sleep(un_paquete);


//             check_contexto(s_cpu_kernel.contexto, dummy_ctx);
//             should_string(s_cpu_kernel.solicitud_recurso.args_io_sleep.nombre_io) be equal to(nombre_io);
//             should_int(s_cpu_kernel.solicitud_recurso.args_io_sleep.unidades_trabajo) be equal to(unidades_trabajo);
//         }end

//     }end

//     describe("Mensaje con op_code CPU_KERNEL_DESALOJO_IO_STDIN_READ") {
//         t_contexto_ejecucion dummy_ctx;
//         t_paquete *un_paquete;
//         s_cpu_kernel s_cpu_kernel;
//         before {
//             set_context(&dummy_ctx);
//         }
//         end

//         after
//         {
//         }
//         end

//         it("debería ser serializado y deserializado correctamente")
//         {

//         char *nombre_io = "palermo";

//         //-----ENTRADA 1-----
//         t_list *listado_frames = list_create();
//         frames_element *frames_element_pkt1 = malloc(sizeof(frames_element));
//         frames_element_pkt1->pid = 7;
//         frames_element_pkt1->offset = 8;
//         frames_element_pkt1->padding = 8;
//         frames_element_pkt1->frame = 1;
//         frames_element_pkt1->tam_pag = 16;
//         frames_element_pkt1->data = "juano probo esto";
//         list_add(listado_frames, (void *)frames_element_pkt1);
//         //-----ENTRADA 2-----
//         frames_element *frames_element_pkt2 = malloc(sizeof(frames_element));
//         frames_element_pkt2->pid = 7;
//         frames_element_pkt2->offset = 0;
//         frames_element_pkt2->padding = 16;
//         frames_element_pkt2->frame = 2;
//         frames_element_pkt2->tam_pag = 16;
//         frames_element_pkt2->data = "juano intento ??";
//         list_add(listado_frames, (void *)frames_element_pkt2);
//         //-----ENTRADA 3-----
//         frames_element *frames_element_pkt3 = malloc(sizeof(frames_element));
//         frames_element_pkt3->pid = 7;
//         frames_element_pkt3->offset = 0;
//         frames_element_pkt3->padding = 14;
//         frames_element_pkt3->frame = 0;
//         frames_element_pkt3->tam_pag = 16;
//         frames_element_pkt3->data = "juano logro esto";
//         uint32_t size = frames_element_pkt3->tam_pag - frames_element_pkt3->padding - frames_element_pkt3->offset;
//         list_add(listado_frames, (void *)frames_element_pkt3);

//         s_cpu_kernel = create_s_cpu_kernel_io_stdin(dummy_ctx, nombre_io, listado_frames, size);
//         un_paquete = serializar_cpu_kernel_io_stdin(s_cpu_kernel);
//         should_int(un_paquete->codigo_operacion) be equal to(CPU_KERNEL_DESALOJO_IO_STDIN_READ);
//         s_cpu_kernel = deserializar_cpu_kernel_io_stdin(un_paquete);
//         check_contexto(s_cpu_kernel.contexto, dummy_ctx);
//         should_string(s_cpu_kernel.solicitud_recurso.args_io_stdin.nombre_io) be equal to(nombre_io);
//         frames_element *resultado = list_get(s_cpu_kernel.solicitud_recurso.args_io_stdin.lista_frames, 0);


//         //Primer frame
//         should_int(resultado->pid) be equal to(7);
//         should_int(resultado->frame) be equal to(1);
//         // should_int(memcmp("juano probo esto", resultado->data, 16)) be equal to (0); // esto ahora no se serializa ni deserliazia

        
//         //Segundo frame
//         resultado = list_get(s_cpu_kernel.solicitud_recurso.args_io_stdin.lista_frames, 1);
//         should_int(resultado->pid) be equal to(7);
//         should_int(resultado->frame) be equal to(2);
//         // should_int(memcmp("juano intento ??", resultado->data, 16)) be equal to (0); // esto ahora no se serializa ni deserliazia

        
//         //Segundo frame
//         resultado = list_get(s_cpu_kernel.solicitud_recurso.args_io_stdin.lista_frames, 2);
//         should_int(resultado->pid) be equal to(7);
//         should_int(resultado->frame) be equal to(0);
//         // should_int(memcmp("juano logro esto", resultado->data, 16)) be equal to (0); // esto ahora no se serializa ni deserliazia

        
//         }
//         end
//     }
//     end

// //    describe("Mensaje con op_code CPU_KERNEL_DESALOJO_IO_STDOUT_WRITE") {
// //        t_contexto_ejecucion dummy_ctx;
// //        t_paquete * un_paquete;
// //        s_cpu_kernel s_cpu_kernel;
// //        before {
// //            set_context(&dummy_ctx);
// //        }end
// //
// //        after {
// //        }end
// //
// //        it("debería ser serializado y deserializado correctamente"){
// //
// //            char* nombre_io = "palermo";
// //            uint32_t una_direccion = 10;
// //            uint32_t un_tamano = 12;
// //
// //            s_cpu_kernel = create_s_cpu_kernel_io_stdout(dummy_ctx,una_direccion , nombre_io,un_tamano  );
// //
// //            un_paquete = serializar_cpu_kernel_io_stdout(s_cpu_kernel);
// //            should_int(un_paquete->codigo_operacion) be equal to(CPU_KERNEL_DESALOJO_IO_STDOUT_WRITE);
// //            s_cpu_kernel =  deserializar_cpu_kernel_io_stdout(un_paquete);
// //
// //
// //            check_contexto(s_cpu_kernel.contexto, dummy_ctx);
// //            should_string(s_cpu_kernel.solicitud_recurso.args_io_stdout.nombre_io) be equal to(nombre_io);
// //            should_int(s_cpu_kernel.solicitud_recurso.args_io_stdout.direccion_traducida) be equal to(una_direccion);
// //            should_int(s_cpu_kernel.solicitud_recurso.args_io_stdout.tamano) be equal to(un_tamano);
// //        }end
// //
// //    }end

//     describe("Mensaje con op_code CPU_KERNEL_DESALOJO_IO_FS_CREATE") {
//         t_contexto_ejecucion dummy_ctx;
//         t_paquete * un_paquete;
//         s_cpu_kernel s_cpu_kernel;
//         before {
//             set_context(&dummy_ctx);
//         }end

//         after {
//         }end

//         it("debería ser serializado y deserializado correctamente"){

//             char* nombre_io = "palermo";
//             char* nombre_archivo = "xeneize";

//             s_cpu_kernel = create_s_cpu_kernel_io_fs_create(nombre_io, dummy_ctx, nombre_archivo   );

//             un_paquete = serializar_cpu_kernel_io_fs_create(s_cpu_kernel);
//             should_int(un_paquete->codigo_operacion) be equal to(CPU_KERNEL_DESALOJO_IO_FS_CREATE);
//             s_cpu_kernel =  deserializar_cpu_kernel_io_fs_create(un_paquete);


//             check_contexto(s_cpu_kernel.contexto, dummy_ctx);
//             should_string(s_cpu_kernel.solicitud_recurso.args_io_fs_create.nombre_io  ) be equal to(nombre_io);
//             should_string(s_cpu_kernel.solicitud_recurso.args_io_fs_create.nombre_archivo  ) be equal to(nombre_archivo);
//         }end

//     }end

//     describe("Mensaje con op_code CPU_KERNEL_DESALOJO_IO_FS_DELETE") {
//         t_contexto_ejecucion dummy_ctx;
//         t_paquete * un_paquete;
//         s_cpu_kernel s_cpu_kernel;
//         before {
//             set_context(&dummy_ctx);
//         }end

//         after {
//         }end

//         it("debería ser serializado y deserializado correctamente"){

//             char* nombre_io = "palermo";
//             char* nombre_archivo = "xeneize";

//             s_cpu_kernel = create_s_cpu_kernel_io_fs_delete(nombre_io, dummy_ctx, nombre_archivo   );

//             un_paquete = serializar_cpu_kernel_io_fs_delete(s_cpu_kernel);
//             should_int(un_paquete->codigo_operacion) be equal to(CPU_KERNEL_DESALOJO_IO_FS_DELETE);
//             s_cpu_kernel =  deserializar_cpu_kernel_io_fs_delete(un_paquete);


//             check_contexto(s_cpu_kernel.contexto, dummy_ctx);
//             should_string(s_cpu_kernel.solicitud_recurso.args_io_fs_delete.nombre_io  ) be equal to(nombre_io);
//             should_string(s_cpu_kernel.solicitud_recurso.args_io_fs_delete.nombre_archivo  ) be equal to(nombre_archivo);
//         }end

//     }end

//     describe("Mensaje con op_code CPU_KERNEL_DESALOJO_IO_FS_TRUNCATE") {
//         t_contexto_ejecucion dummy_ctx;
//         t_paquete * un_paquete;
//         s_cpu_kernel s_cpu_kernel;
//         before {
//             set_context(&dummy_ctx);
//         }end

//         after {
//         }end

//         it("debería ser serializado y deserializado correctamente"){

//             char* nombre_io = "palermo";
//             char* nombre_archivo = "xeneize";
//             uint32_t un_tamano = 32;

//             s_cpu_kernel = create_s_cpu_kernel_io_fs_truncate(dummy_ctx, nombre_io, un_tamano, nombre_archivo   );

//             un_paquete = serializar_cpu_kernel_io_fs_truncate(s_cpu_kernel);
//             should_int(un_paquete->codigo_operacion) be equal to(CPU_KERNEL_DESALOJO_IO_FS_TRUNCATE);
//             s_cpu_kernel =  deserialziar_cpu_kernel_io_fs_truncate(un_paquete);


//             check_contexto(s_cpu_kernel.contexto, dummy_ctx);
//             should_string(s_cpu_kernel.solicitud_recurso.args_io_fs_truncate.nombre_io  ) be equal to(nombre_io);
//             should_string(s_cpu_kernel.solicitud_recurso.args_io_fs_truncate.nombre_archivo  ) be equal to(nombre_archivo);
//             should_int(s_cpu_kernel.solicitud_recurso.args_io_fs_truncate.tamano  ) be equal to(un_tamano);
//         }end

//     }end

//     describe("Mensaje con op_code CPU_KERNEL_DESALOJO_IO_FS_WRITE") {
//         t_contexto_ejecucion dummy_ctx;
//         t_paquete * un_paquete;
//         s_cpu_kernel s_cpu_kernel;
//         t_list *info_marcos;

//         before {
//             set_context(&dummy_ctx);
//             info_marcos = list_create();
//             //-----ENTRADA 1-----
//             frames_element *frames_element_pkt1 = malloc(sizeof(frames_element));
//             frames_element_pkt1->pid = 7;
//             frames_element_pkt1->offset = 8;
//             frames_element_pkt1->padding = 8;
//             frames_element_pkt1->frame = 1;
//             frames_element_pkt1->tam_pag = 16;
//             list_add(info_marcos, (void *)frames_element_pkt1);
//             //-----ENTRADA 2-----
//             frames_element *frames_element_pkt2 = malloc(sizeof(frames_element));
//             frames_element_pkt2->pid = 7;
//             frames_element_pkt2->offset = 0;
//             frames_element_pkt2->padding = 16;
//             frames_element_pkt2->frame = 2;
//             frames_element_pkt2->tam_pag = 16;
//             list_add(info_marcos, (void *)frames_element_pkt2);
//         }end

//         after {
//             list_destroy(info_marcos);
//         }end

//         it("debería ser serializado y deserializado correctamente"){

//             char* nombre_io = "palermo";
//             char* nombre_archivo = "xeneize";
//             uint32_t una_direccion = 23;
//             uint32_t un_tamano = 32;
//             uint32_t un_puntero = 2323;

//             s_cpu_kernel = create_s_cpu_kernel_io_fs_write( nombre_io,  un_tamano, dummy_ctx,info_marcos,nombre_archivo,un_puntero   );

//             un_paquete = serializar_cpu_kernel_io_fs_write(s_cpu_kernel);
//             should_int(un_paquete->codigo_operacion) be equal to(CPU_KERNEL_DESALOJO_IO_FS_WRITE);
//             s_cpu_kernel =  deserializar_cpu_kernel_io_fs_write(un_paquete);


//             check_contexto(s_cpu_kernel.contexto, dummy_ctx);
//             should_string(s_cpu_kernel.solicitud_recurso.args_io_fs_write.nombre_io  ) be equal to(nombre_io);
//             should_string(s_cpu_kernel.solicitud_recurso.args_io_fs_write.nombre_archivo  ) be equal to(nombre_archivo);

//             frames_element *resultado = list_get(s_cpu_kernel.solicitud_recurso.args_io_fs_write.info_marcos, 0);
//             //Primer frame
//             should_int(resultado->pid) be equal to(7);
//             should_int(resultado->frame) be equal to(1);
//             //Segundo frame
//             resultado = list_get(s_cpu_kernel.solicitud_recurso.args_io_fs_write.info_marcos, 1);
//             should_int(resultado->pid) be equal to(7);
//             should_int(resultado->frame) be equal to(2);

//             should_int(s_cpu_kernel.solicitud_recurso.args_io_fs_write.tamano  ) be equal to(un_tamano);
//             should_int(s_cpu_kernel.solicitud_recurso.args_io_fs_write.puntero_archivo  ) be equal to(un_puntero    );
//         }end

//     }end

//     describe("Mensaje con op_code CPU_KERNEL_DESALOJO_IO_FS_READ") {
//         t_contexto_ejecucion dummy_ctx;
//         t_paquete * un_paquete;
//         s_cpu_kernel s_cpu_kernel;
//         t_list *direcciones_fisicas;

//         before {
//             set_context(&dummy_ctx);
//             direcciones_fisicas = list_create();
//             //-----ENTRADA 1-----
//             frames_element *frames_element_pkt1 = malloc(sizeof(frames_element));
//             frames_element_pkt1->pid = 7;
//             frames_element_pkt1->offset = 8;
//             frames_element_pkt1->padding = 8;
//             frames_element_pkt1->frame = 1;
//             frames_element_pkt1->tam_pag = 16;
//             list_add(direcciones_fisicas, (void *)frames_element_pkt1);
//             //-----ENTRADA 2-----
//             frames_element *frames_element_pkt2 = malloc(sizeof(frames_element));
//             frames_element_pkt2->pid = 7;
//             frames_element_pkt2->offset = 0;
//             frames_element_pkt2->padding = 16;
//             frames_element_pkt2->frame = 2;
//             frames_element_pkt2->tam_pag = 16;
//             list_add(direcciones_fisicas, (void *)frames_element_pkt2);
//         }end

//         after {
//             list_destroy(direcciones_fisicas);
//         }end

//         it("debería ser serializado y deserializado correctamente"){

//             char* nombre_io = "palermo";
//             char* nombre_archivo = "xeneize";
//             uint32_t un_tamano = 32;
//             uint32_t un_puntero = 2323;

//             s_cpu_kernel = create_s_cpu_kernel_io_fs_read( nombre_io,  un_tamano, dummy_ctx,direcciones_fisicas,nombre_archivo,un_puntero   );

//             un_paquete = serializar_cpu_kernel_io_fs_read(s_cpu_kernel);
//             should_int(un_paquete->codigo_operacion) be equal to(CPU_KERNEL_DESALOJO_IO_FS_READ);
//             s_cpu_kernel =  deserializar_cpu_kernel_io_fs_read(un_paquete);


//             check_contexto(s_cpu_kernel.contexto, dummy_ctx);
//             should_string(s_cpu_kernel.solicitud_recurso.args_io_fs_read.nombre_io  ) be equal to(nombre_io);
//             should_string(s_cpu_kernel.solicitud_recurso.args_io_fs_read.nombre_archivo  ) be equal to(nombre_archivo);
           
//             frames_element *resultado = list_get(s_cpu_kernel.solicitud_recurso.args_io_fs_read.info_marcos, 0);
//             //Primer frame
//             should_int(resultado->pid) be equal to(7);
//             should_int(resultado->frame) be equal to(1);
//             //Segundo frame
//             resultado = list_get(s_cpu_kernel.solicitud_recurso.args_io_fs_read.info_marcos, 1);
//             should_int(resultado->pid) be equal to(7);
//             should_int(resultado->frame) be equal to(2);

//             should_int(s_cpu_kernel.solicitud_recurso.args_io_fs_read.tamano  ) be equal to(un_tamano);
//             should_int(s_cpu_kernel.solicitud_recurso.args_io_fs_read.puntero_archivo  ) be equal to(un_puntero    );
//         }end

//     }end
// }