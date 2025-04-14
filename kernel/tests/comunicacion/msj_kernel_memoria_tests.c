#include "serializacion/mensajes/kernel_memoria/mensajes_kernel_memoria.h"
#include "serializacion/serializacion.h"
#include <TAD/TAD.h>
#include <cspecs/cspec.h>

context(msj_kernel_cpu) {

    describe("Mensaje con op_code KERNEL_MEMORIA_FINALIZAR_PROCESO y pid 8"){
        
        it("Debería ser serializado y al deserealizar devolver el mismo pid"){
            uint32_t pid_a_enviar = 8;
            t_paquete* paquete_feo = finalizar_proceso_memoria(pid_a_enviar);
            should_int((op_code)paquete_feo->codigo_operacion) be equal to((op_code)KERNEL_MEMORIA_FINALIZAR_PROCESO);
            should_int((int)paquete_feo->buffer->size) be equal to(sizeof(int));
            uint32_t pid_decereal = 999999;
            deserializar_uint32_t(paquete_feo->buffer, &pid_decereal);
            should_int((int)pid_a_enviar) be equal to((int)pid_decereal);
            paquete_destroy(paquete_feo);
        }end

    }end

    describe("Mensaje con op_code KERNEL_MEMORIA_CREAR_PROCESO, pid 8 y pseudocodigo 'jajasalu2'"){
            
        it("Debería ser serializado y al deserealizar devolver el mismo pid y mismo path"){
            uint32_t pid_a_enviar = 8;
            char* path_a_enviar = "jajasalu2";
            s_kernel_memoria_creacion_proceso gusty_lpm = create_k_m_creacion_proceso(pid_a_enviar, path_a_enviar);
            t_paquete* paquete_feo = serializar_k_m_creacion_proceso(gusty_lpm);
            should_int((op_code)paquete_feo->codigo_operacion) be equal to((op_code)KERNEL_MEMORIA_CREAR_PROCESO);
            uint32_t pid_decereal = 999999;
            char* path_decereal;
            deserializar_uint32_t(paquete_feo->buffer, &pid_decereal);
            deserializar_string(paquete_feo->buffer, &path_decereal);
            should_int((int)pid_a_enviar) be equal to((int)pid_decereal);
            should_string((char *)path_a_enviar) be equal to((char *)path_decereal);
            paquete_destroy(paquete_feo);
        }end

    }end
}