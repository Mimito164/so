// #include "serializacion/mensajes/kernel_io/msj_kernel_io.h"
// #include "serializacion/serializacion.h"
// #include "stdlib.h"
// #include <cspecs/cspec.h>
// #include "TAD/TAD.h"

// context(msj_kernel_io) {

//     uint32_t pid = 1;
//     char * file_name = "archivo.txt";
//     uint32_t real_mem_dir = 666;
//     uint32_t size = 6;

//     t_paquete * paquete;

//     describe("Mensaje con op_code KERNEL_IO_GEN_ZZZ"){

//         s_kernel_io_sleep s_knel_io_sleep;

//         uint32_t work_unit = 8;

//         before {
//             s_knel_io_sleep = create_kernel_io_sleep_struct(pid, work_unit);
//         }end

//         after {
//             paquete_destroy(paquete);
//         }end

//         it("debería ser serializado y deserializado correctamente"){
//             paquete = serializar_kernel_io_sleep_struct(s_knel_io_sleep);
//             s_knel_io_sleep = deserializar_kernel_io_sleep_struct(paquete);

//             should_int((int)s_knel_io_sleep.pid) be equal to((int)pid);
//             should_int((int)s_knel_io_sleep.work_unit) be equal to((int)work_unit);
//         }end
//     }end

//     describe("Mensaje con op_code KERNEL_IO_STDIN_READ"){


//         s_kernel_io_stdin_read s_knel_io_stdin_read;

//         before {
            
//         }end

//         after {
//             paquete_destroy(paquete);
//         }end

//         it("debería ser serializado y deserializado correctamente"){
//             t_list *listado_frames = list_create();

//             //-----ENTRADA 1-----
//             frames_element *mem_transfer_pkt1 = malloc(sizeof(frames_element));
//             mem_transfer_pkt1->pid = 7;
//             mem_transfer_pkt1->offset = 8;
//             mem_transfer_pkt1->padding = 8;
//             mem_transfer_pkt1->frame = 1;
//             mem_transfer_pkt1->tam_pag = 16;
//             mem_transfer_pkt1->data = "juano probo esto";
//             list_add(listado_frames, (void *)mem_transfer_pkt1);

//             //-----ENTRADA 2-----
//             frames_element *mem_transfer_pkt2 = malloc(sizeof(frames_element));
//             mem_transfer_pkt2->pid = 7;
//             mem_transfer_pkt2->offset = 0;
//             mem_transfer_pkt2->padding = 16;
//             mem_transfer_pkt2->frame = 2;
//             mem_transfer_pkt2->tam_pag = 16;
//             mem_transfer_pkt2->data = "juano intento ??";
//             list_add(listado_frames, (void *)mem_transfer_pkt2);

//             //-----ENTRADA 3-----
//             frames_element *mem_transfer_pkt3 = malloc(sizeof(frames_element));
//             mem_transfer_pkt3->pid = 7;
//             mem_transfer_pkt3->offset = 0;
//             mem_transfer_pkt3->padding = 14;
//             mem_transfer_pkt3->frame = 0;
//             mem_transfer_pkt3->tam_pag = 16;
//             mem_transfer_pkt3->data = "juano logro esto";
//             uint32_t size = mem_transfer_pkt3->tam_pag - mem_transfer_pkt3->padding - mem_transfer_pkt3->offset;
//             list_add(listado_frames, (void *)mem_transfer_pkt3);

//             s_kernel_io_stdin_read estructura = create_kernel_io_stdin_read(pid, listado_frames, size);
//             paquete = serializar_kernel_io_stdin_read(estructura);
//             should_int(paquete->codigo_operacion) be equal to(KERNEL_IO_STDIN_READ);
//             s_knel_io_stdin_read = deserializar_kernel_io_stdin_read(paquete);

//             frames_element *resultado = list_get(s_knel_io_stdin_read.lista_frames, 0);
//             //Primer frame
//             should_int(resultado->pid) be equal to(7);
//             should_int(resultado->frame) be equal to(1);
//             // should_int(memcmp("juano probo esto", resultado->data, 16)) be equal to (0); // esto ahora no se serializa ni deserliazia

//             //Segundo frame
//             resultado = list_get(s_knel_io_stdin_read.lista_frames, 1);
//             should_int(resultado->pid) be equal to(7);
//             should_int(resultado->frame) be equal to(2);
//             // should_int(memcmp("juano intento ??", resultado->data, 16)) be equal to (0); // esto ahora no se serializa ni deserliazia

//             //Segundo frame
//             resultado = list_get(s_knel_io_stdin_read.lista_frames, 2);
//             should_int(resultado->pid) be equal to(7);
//             should_int(resultado->frame) be equal to(0);
//             // should_int(memcmp("juano logro esto", resultado->data, 16)) be equal to (0); // esto ahora no se serializa ni deserliazia

//         }end


//     }end

//   /*  describe("Mensaje con op_code KERNEL_IO_STDOUT_WRITE"){


//         s_kernel_io_stdout_write s_knel_io_stdout_write;

//         before {
//             s_knel_io_stdout_write = create_kernel_io_stdout_write(pid, real_mem_dir, size);
//         }end

//         after {
//             paquete_destroy(paquete);
//         }end

//         it("debería ser serializado y deserializado correctamente"){
//             paquete = serializar_kernel_io_stdout_write(s_knel_io_stdout_write);
//             s_knel_io_stdout_write = deserializar_kernel_io_stdout_write(paquete);

//             should_int((int)s_knel_io_stdout_write.pid) be equal to((int)pid);
//             should_int((int)s_knel_io_stdout_write.real_mem_dir) be equal to((int)real_mem_dir);
//             should_int((int)s_knel_io_stdout_write.size) be equal to((int)size);

//         }end

//     }end
// */
//     describe("Mensaje con op_code IO_FS_DELETE"){

//         s_kernel_io_fs_delete s_knel_io_fs_delete;

//         before {
//             s_knel_io_fs_delete = create_kernel_io_fs_delete(pid, file_name);
//         }end

//         after {
//             paquete_destroy(paquete);
//         }end

//         it("debería ser serializado y deserializado correctamente"){
//             paquete = serializar_kernel_io_fs_delete(s_knel_io_fs_delete);
//             s_knel_io_fs_delete = deserializar_kernel_io_fs_delete(paquete);

//             should_int((int)s_knel_io_fs_delete.pid) be equal to((int)pid);
//             should_string(s_knel_io_fs_delete.file_name) be equal to(file_name);

//         }end

//     }end


//     describe("Mensaje con op_code IO_FS_TRUNCATE"){


//         s_kernel_io_fs_truncate s_knel_io_fs_truncate;

//         before {
//             s_knel_io_fs_truncate = create_kernel_io_fs_truncate(pid, file_name, size);
//         }end

//         after {
//             paquete_destroy(paquete);
//         }end

//         it("debería ser serializado y deserializado correctamente"){
//             paquete = serializar_kernel_io_fs_truncate(s_knel_io_fs_truncate);
//             s_knel_io_fs_truncate = deserializar_kernel_io_fs_truncate(paquete);

//             should_int((int)s_knel_io_fs_truncate.pid) be equal to((int)pid);
//             should_string(s_knel_io_fs_truncate.file_name) be equal to(file_name);
//             should_int((int)s_knel_io_fs_truncate.new_size) be equal to((int)size);

//         }end

//     }end

//     describe("Mensaje con op_code IO_FS_WRITE"){

//         s_kernel_io_fs_write s_knel_io_fs_write;
//         uint32_t file_pointer = 9;
//         t_list *info_marcos;

//         before {
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
//             s_knel_io_fs_write = create_kernel_io_fs_write(pid, file_name, file_pointer, info_marcos, size);
//         }end

//         after {
//             paquete_destroy(paquete);
//             list_destroy(info_marcos);
//         }end

//         it("debería ser serializado y deserializado correctamente"){
//             paquete = serializar_kernel_io_fs_write(s_knel_io_fs_write);
//             s_knel_io_fs_write = deserializar_kernel_io_fs_write(paquete);

//             should_int((int)s_knel_io_fs_write.pid) be equal to((int)pid);
//             should_string(s_knel_io_fs_write.file_name) be equal to(file_name);
//             should_int((int)s_knel_io_fs_write.file_pointer) be equal to((int)file_pointer);

//             frames_element *resultado = list_get(s_knel_io_fs_write.info_marcos, 0);
//             //Primer frame
//             should_int(resultado->pid) be equal to(7);
//             should_int(resultado->frame) be equal to(1);
//             //Segundo frame
//             resultado = list_get(s_knel_io_fs_write.info_marcos, 1);
//             should_int(resultado->pid) be equal to(7);
//             should_int(resultado->frame) be equal to(2);

//             should_int((int)s_knel_io_fs_write.size) be equal to((int)size);

//         }end

//     }end

//     describe("Mensaje con op_code IO_FS_READ"){

//         s_kernel_io_fs_read s_knel_io_fs_read;
//         uint32_t file_pointer = 9;
//         t_list *info_marcos;

//         before {
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
//             s_knel_io_fs_read = create_kernel_io_fs_read(pid, file_name, file_pointer, info_marcos, size);
//         }end

//         after {
//             paquete_destroy(paquete);
//             list_destroy(info_marcos);
//         }end

//         it("debería ser serializado y deserializado correctamente"){
//             paquete = serializar_kernel_io_fs_read(s_knel_io_fs_read);
//             s_knel_io_fs_read = deserializar_kernel_io_fs_read(paquete);

//             should_int((int)s_knel_io_fs_read.pid) be equal to((int)pid);
//             should_string(s_knel_io_fs_read.file_name) be equal to(file_name);
//             should_int((int)s_knel_io_fs_read.file_pointer) be equal to((int)file_pointer);

//             frames_element *resultado = list_get(s_knel_io_fs_read.info_marcos, 0);
//             //Primer frame
//             should_int(resultado->pid) be equal to(7);
//             should_int(resultado->frame) be equal to(1);
//             //Segundo frame
//             resultado = list_get(s_knel_io_fs_read.info_marcos, 1);
//             should_int(resultado->pid) be equal to(7);
//             should_int(resultado->frame) be equal to(2);

//             should_int((int)s_knel_io_fs_read.size) be equal to((int)size);

//         }end

//     }end
// }