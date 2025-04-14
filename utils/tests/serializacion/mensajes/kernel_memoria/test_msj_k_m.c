// #include "serializacion/mensajes/kernel_memoria/mensajes_kernel_memoria.h"
// #include "serializacion/serializacion.h"
// #include <cspecs/cspec.h>

// context(msj_k_m) {

//     describe("Mensaje con op_code KERNEL_MEMORIA_CREAR_PROCESO"){
        
//         uint32_t pid = 2;
//         char* path="./pepito/pepito.txt";
//         t_paquete * paquete;
//         s_kernel_memoria_creacion_proceso s_k_m_creacion_pros;
//         before {
//             s_k_m_creacion_pros=create_k_m_creacion_proceso(pid, path);
//         }end

//         after {
//             paquete_destroy(paquete);
//         }end
        
//         it("debería ser serializado y deserializado correctamente"){
//             paquete = serializar_k_m_creacion_proceso(s_k_m_creacion_pros);

//             s_k_m_creacion_pros = deserializar_k_m_creacion_proceso(paquete);
//             should_int((int) s_k_m_creacion_pros.pid) be equal to((int)pid);
//             should_string(s_k_m_creacion_pros.path) be equal to(path);
//         }end

//     }end
    
// }