#include <comunicacion_kernel/resolver.h>
#include "serializacion/serializacion.h"
#include "serializacion/mensajes/kernel_io/msj_kernel_io.h"
#include "serializacion/mensajes/io_memoria/mensajes_io_memoria.h"
#include <fcntl.h>
#include "utils/console.h"
#include <cspecs/cspec.h>


//    context(Delegante)
//    {
//        describe("Resolver")
//        {
          
//            before{
//                initialize_cfg("tests/testing.config");
//                consola_con_censura();
//            }
//            end 
//
//            after{
//                
//            }
//            end 
//
//            it("debería devolver un paquete con opcode IO_KERNEL_STDIN_READ para enviar a kernel")
//            {
//                uint32_t pid = 4;
//                uint32_t real_mem_dir = 10; 
//                uint32_t size = 5;
//                s_kernel_io_stdin_read estructura1 = create_kernel_io_stdin_read(pid, real_mem_dir, size);
//                t_paquete * paqueteKernel = serializar_kernel_io_stdin_read(estructura1);
//                
//                int stdin_fd = dup(STDIN_FILENO);
//                int user_input_fd = open("tests/comunicacion_kernel/textInput", O_RDONLY); // consola_test_input tiene lo que el usuario "escribe"
//                dup2(user_input_fd, STDIN_FILENO);
//                close(user_input_fd);
//
//                t_paquete* paquete1 = resolver_KERNEL_IO_STDIN_READ(paqueteKernel);
//
//                dup2(stdin_fd, STDIN_FILENO);
//                close(stdin_fd);
//                consola_con_libre_expresion();
//                should_int(paquete1->codigo_operacion) be equal to(IO_KERNEL_STDIN_READ);
//                paquete_destroy(paquete1);
//                s_io_memoria_write estructura2 = deserializar_io_memoria_write(paqueteEnviarMemoria_test);
//                should_int(estructura2.real_mem_dir) be equal to(real_mem_dir);
//                should_int(estructura2.size) be equal to(size);
//                should_string(estructura2.info) be equal to("Hola");
//
//            }
//            end
//            it("debería devolver un paquete con opcode resolver_KERNEL_IO_STDOUT_WRITE para enviar a kernel")
//            {
//                uint32_t pid = 4;
//                uint32_t real_mem_dir = 10; 
//                uint32_t size = 5;
//                s_kernel_io_stdout_write estructura1 = create_kernel_io_stdout_write(pid, real_mem_dir, size);
//                t_paquete * paqueteKernel = serializar_kernel_io_stdout_write(estructura1);
//                
//                t_paquete* paquete1 = resolver_KERNEL_IO_STDOUT_WRITE(paqueteKernel);
//
//                consola_con_libre_expresion();
//                should_int(paquete1->codigo_operacion) be equal to(IO_KERNEL_STDOUT_WRITE);
//                paquete_destroy(paquete1);
//                s_io_memoria_read estructura2 = deserializar_io_memoria_read(paqueteEnviarMemoria_test);
//                should_int(estructura2.real_mem_dir) be equal to(real_mem_dir);
//                should_int(estructura2.size) be equal to(size);
//
//
//            }
//            end
//        }
//        end
//    }