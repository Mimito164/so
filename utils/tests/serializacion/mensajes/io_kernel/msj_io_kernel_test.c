// #include "serializacion/mensajes/io_kernel/msj_io_kernel.h"
// #include "serializacion/serializacion.h"
// #include <cspecs/cspec.h>

//     context(msj_io_kernel)
//     {
//         describe("Mensaje con op_code send_io_kernel_INICIAR_CONEXION")
//         {
//             char *nombreDeIO = "ImpresoraDelDiegote";
//             s_io_kernel_iniciar_conexion resultadoDeDeserializar;

//             before{
//                 // nada
//             }
//             end 

//             after{
//             }
//             end 

//             it("debería ser serializado y deserializado correctamente")
//             {
//                 s_io_kernel_iniciar_conexion s_io_kernel_init_conec;
//                 s_io_kernel_init_conec.nombreDeIO = nombreDeIO;
//                 s_io_kernel_init_conec.tipo = GENERICA ;
//                 t_paquete *el_paquete_del_diego = serializar_io_kernel_INICIAR_CONEXION(s_io_kernel_init_conec);
                
//                 resultadoDeDeserializar = deserializar_io_kernel_INICIAR_CONEXION(el_paquete_del_diego);
//                 should_string(resultadoDeDeserializar.nombreDeIO) be equal to(nombreDeIO);
//             }
//             end
//         }
//         end
//     }