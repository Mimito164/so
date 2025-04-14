// #include "serializacion/mensajes/memoria_io/msj_memoria_io.h"
// #include "serializacion/serializacion.h"
// #include <cspecs/cspec.h>

// context(msj_memoria_io) {

//     describe("Mensaje con op_code MEMORIA_IO_WRITE"){

//         uint32_t size = 10;
//         void *value = "El Diegote";
//         uint32_t position = 10;
//         s_memoria_io_write s_mem_io_write;

//         before {
//             s_mem_io_write = create_mem_io_write_struct(size, value, position);
//         }end

//         it("debería ser serializado y deserializado correctamente"){
//             t_paquete * el_paquete_del_diego = serializar_m_i_write_struct(s_mem_io_write);
//             should_int((int)el_paquete_del_diego->codigo_operacion) be equal to((int)MEMORIA_IO_WRITE);
//             s_memoria_io_write struct_del_diego = deserializar_m_i_write_struct(el_paquete_del_diego);
//             should_int((int)struct_del_diego.size) be equal to((int)size);
//             should_int((int)struct_del_diego.position) be equal to((int)position);
//             should_string((char *)struct_del_diego.value) be equal to((char *)value);
//         }end

//     }end

//     describe("Mensaje con op_code MEMORIA_IO_READ"){
//         it("debería ser serializado y deserializado correctamente"){

//             mem_io_status staus;
//             t_paquete * paquete = NULL; 
//             mem_io_status post_status; 

//             staus = OK;
//             paquete = serializar_m_i_read(staus);
//             should_int((int)paquete->codigo_operacion) be equal to((int)MEMORIA_IO_READ);
//             post_status = deserializar_m_i_read(paquete);
//             should_int(post_status) be equal to(staus);

//             staus = NOTOK;
//             paquete = serializar_m_i_read(staus);
//             post_status = deserializar_m_i_read(paquete);
//             should_int(post_status) be equal to(staus);

//             paquete_destroy(paquete);
//         }end
//     }end
// }
	