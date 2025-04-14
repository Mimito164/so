// #include "serializacion/mensajes/memoria_cpu/msj_memoria_cpu.h"
// #include "serializacion/serializacion.h"
// #include <cspecs/cspec.h>

// context(msj_memoria_cpu) {

//     describe("Mensaje con op_code CPU_MEMORIA_OBTENER_MARCO"){

//         uint32_t pid_marco = 2;
//         uint32_t pag = 8;
//         t_paquete * paquete_marco;
//         s_memoria_cpu_marco s_mem_cpu_marco;

//         before {
//             s_mem_cpu_marco = create_mem_cpu_marco_struct(pid_marco,pag);
//         }end

//         after {
//             paquete_destroy(paquete_marco);
//         }end

//         it("debería ser serializado y deserializado correctamente"){
//             paquete_marco = serializar_c_m_marco(s_mem_cpu_marco);

//             s_mem_cpu_marco = deserializar_m_c_marco_struct(paquete_marco);
//             should_int((int)s_mem_cpu_marco.pid) be equal to((int)pid_marco);
//             should_int((int)s_mem_cpu_marco.pag) be equal to((int)pag);
//         }end

//     }end

//     describe("Mensaje con op_code CPU_MEMORIA_RESIZE"){

//         uint32_t pid_resize = 5;
//         uint32_t size = 15;
//         t_paquete *paquete_resize;
//         s_memoria_cpu_resize s_mem_cpu_resize;

//         before {
//             s_mem_cpu_resize = create_mem_cpu_resize_struct(pid_resize,size);
//         }end

//         after {
//             paquete_destroy(paquete_resize);
//         }end

//         it("debería ser serializado y deserializado correctamente"){
//             paquete_resize = serializar_c_m_resize(s_mem_cpu_resize);

//             s_mem_cpu_resize = deserializar_m_c_resize_struct(paquete_resize);
//             should_int((int)s_mem_cpu_resize.pid) be equal to((int)pid_resize);
//             should_int((int)s_mem_cpu_resize.size) be equal to((int)size);

//         }end
//     }end

//     describe("Mensaje con op_code CPU_MEMORIA_OBTENER_INSTRUCCIONES"){

//         uint32_t pid_int = 10;
//         uint32_t pc = 5;
//         t_paquete *paquete_int;
//         s_memoria_cpu_instruccion s_mem_cpu_instruccion;

//         before {
//             s_mem_cpu_instruccion = create_mem_cpu_instruccion_struct(pid_int,pc);
//         }end

//         after {
//             paquete_destroy(paquete_int);
//         }end

//         it("debería ser serializado y deserializado correctamente"){
//             paquete_int = serializar_c_m_instruccion(s_mem_cpu_instruccion);

//             s_mem_cpu_instruccion = deserializar_m_c_instruccion_struct(paquete_int);
//             should_int((int)s_mem_cpu_instruccion.pid) be equal to((int)pid_int);
//             should_int((int)s_mem_cpu_instruccion.program_counter) be equal to((int)pc);

//         }end
//     }end

// }