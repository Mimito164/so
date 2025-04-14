// #include "serializacion/mensajes/io_memoria/mensajes_io_memoria.h"
// #include "serializacion/serializacion.h"
// #include <cspecs/cspec.h>

//     context(msj_io_memoria)
//     {
//         describe("Mensaje con op_code IO_MEMORIA_WRITE")
//         {
//             before{
//                 // nada
//             }
//             end 

//             after{
//             }
//             end 

//             it("debería ser serializado y deserializado correctamente")
//             {
//                 s_io_memoria_write estructuraSerializar;
//                 estructuraSerializar.real_mem_dir = 10;
//                 estructuraSerializar.size = 12;
//                 t_paquete *paquete = serializar_io_memoria_write(estructuraSerializar);
//                 s_io_memoria_write estructuraDeserializar = deserializar_io_memoria_write(paquete);
                
//                 should_int(estructuraDeserializar.size) be equal to(estructuraSerializar.size);
//                 should_int(estructuraDeserializar.real_mem_dir) be equal to(estructuraSerializar.real_mem_dir);

//             }
//             end
//         }
//         end
//         describe("Mensaje con op_code IO_MEMORIA_READ")
//         {
//             before{
//                 // nada
//             }
//             end 

//             after{
//             }
//             end 

//             it("debería ser serializado y deserializado correctamente")
//             {
//                 s_io_memoria_read estructuraSerializar;
//                 estructuraSerializar.pid = 10;
//                 estructuraSerializar.real_mem_dir = 10;
//                 estructuraSerializar.size = 12;
//                 estructuraSerializar.data = "LPM Juano >:( ";
//                 t_paquete *paquete = serializar_io_memoria_read(estructuraSerializar);
//                 s_io_memoria_read estructuraDeserializar = deserializar_io_memoria_read(paquete);
                
//                 should_int(estructuraDeserializar.pid) be equal to(estructuraSerializar.pid);
//                 should_int(estructuraDeserializar.size) be equal to(estructuraSerializar.size);
//                 should_int(estructuraDeserializar.real_mem_dir) be equal to(estructuraSerializar.real_mem_dir);
//                 should_string(estructuraSerializar.data) be equal to(estructuraSerializar.data);

//             }
//             end
//         }
//         end
//     }