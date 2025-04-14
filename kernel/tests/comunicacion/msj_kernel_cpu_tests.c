#include "serializacion/mensajes/kernel_cpu/mensajes_kernel_cpu.h"
#include "serializacion/serializacion.h"
#include <TAD/TAD.h>
#include <cspecs/cspec.h>

context(msj_kernel_memoria) {

    describe("Mensaje con op_code KERNEL_CPU_INT_KILL y pid 8"){
        
        it("Debería ser serializado y al deserealizar devolver el mismo pid"){
          uint32_t pid_a_enviar = 8;
          t_paquete* paquete_feo = serializar_kernel_cpu_interrupt(pid_a_enviar, (op_code)KERNEL_CPU_INT_KILL);
          should_int((op_code)paquete_feo->codigo_operacion) be equal to((op_code)KERNEL_CPU_INT_KILL);
          should_int((int)paquete_feo->buffer->size) be equal to(sizeof(int));
          uint32_t pid_decereal = 999999;
          deserializar_uint32_t(paquete_feo->buffer, &pid_decereal);
          should_int((int)pid_a_enviar) be equal to((int)pid_decereal);
          paquete_destroy(paquete_feo);
        }end

    }end

    describe("Mensaje con op_code KERNEL_CPU_INT_DESALOJO y pid 8"){
        
        it("Debería ser serializado y al deserealizar devolver el mismo pid"){
          uint32_t pid_a_enviar = 8;
          t_paquete* paquete_feo = serializar_kernel_cpu_interrupt(pid_a_enviar, (op_code)KERNEL_CPU_INT_DESALOJO);
          should_int((op_code)paquete_feo->codigo_operacion) be equal to((op_code)KERNEL_CPU_INT_DESALOJO);
          should_int((int)paquete_feo->buffer->size) be equal to(sizeof(int));
          uint32_t pid_decereal = 999999;
          deserializar_uint32_t(paquete_feo->buffer, &pid_decereal);
          should_int((int)pid_a_enviar) be equal to((int)pid_decereal);
          paquete_destroy(paquete_feo);
        }end

    }end

    describe("Mensaje con op_code KERNEL_CPU_EXECUTE, pid 8 y contexto de ejecucion"){
            
        it("Debería ser serializado y al deserealizar devolver el mismo pid y mismo contexto de ejecucion"){
          t_registros_cpu registro_a_enviar;
          t_contexto_ejecucion contexto_a_enviar;
          contexto_a_enviar.pid = 8;
          registro_a_enviar.PC = 1;
          registro_a_enviar.AX = 2;
          registro_a_enviar.BX = 3;
          registro_a_enviar.CX = 4;
          registro_a_enviar.DX = 5;
          registro_a_enviar.EAX = 6;
          registro_a_enviar.EBX = 7;
          registro_a_enviar.ECX = 9;
          registro_a_enviar.EDX = 10;
          registro_a_enviar.SI = 11;
          registro_a_enviar.DI = 12;
          contexto_a_enviar.registros_cpu = registro_a_enviar;
          t_paquete* paquete_feo = serializar_kernel_cpu_execute(contexto_a_enviar);
          should_int((op_code)paquete_feo->codigo_operacion) be equal to((op_code)KERNEL_CPU_EXECUTE);
          t_contexto_ejecucion contexto_decereal;
          contexto_decereal = deserializar_contexto_ejecucion(paquete_feo);
          should_int((int)contexto_decereal.pid) be equal to((int)contexto_a_enviar.pid);
          should_int((int)contexto_decereal.registros_cpu.PC) be equal to((int)registro_a_enviar.PC);
          should_int((int)contexto_decereal.registros_cpu.AX) be equal to((int)registro_a_enviar.AX);
          should_int((int)contexto_decereal.registros_cpu.BX) be equal to((int)registro_a_enviar.BX);
          should_int((int)contexto_decereal.registros_cpu.CX) be equal to((int)registro_a_enviar.CX);
          should_int((int)contexto_decereal.registros_cpu.DX) be equal to((int)registro_a_enviar.DX);
          should_int((int)contexto_decereal.registros_cpu.EAX) be equal to((int)registro_a_enviar.EAX);
          should_int((int)contexto_decereal.registros_cpu.EBX) be equal to((int)registro_a_enviar.EBX);
          should_int((int)contexto_decereal.registros_cpu.ECX) be equal to((int)registro_a_enviar.ECX);
          should_int((int)contexto_decereal.registros_cpu.EDX) be equal to((int)registro_a_enviar.EDX);
          should_int((int)contexto_decereal.registros_cpu.SI) be equal to((int)registro_a_enviar.SI);
          should_int((int)contexto_decereal.registros_cpu.DI) be equal to((int)registro_a_enviar.DI);
          paquete_destroy(paquete_feo);
        }end

    }end
}