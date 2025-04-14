#include <stdio.h>
#include <stdbool.h>
#include <cspecs/cspec.h>
#include "handlers/colas_estados/colas_handler.h"

context (PCB_handler) {

    initialize_cfg("tests/testing.config");
    t_config_kernel *cfg = get_config();

    describe("Se crea un pcb") {
        it("Debe crear un pcb con pid 0, quantum del config (el q esta acutalmente en el config) y todos los valores del registro de cpu en 0") {
            gh_lpm();
            t_pcb* new_pcb = create_pcb();
            should_int((int)new_pcb->pid) be equal to(0);
            should_int((int)new_pcb->quantum) be equal to(cfg->QUANTUM);
            should_int((int)new_pcb->registros_cpu->PC) be equal to(0);
            should_int((int)new_pcb->registros_cpu->AX) be equal to(0);
            should_int((int)new_pcb->registros_cpu->BX) be equal to(0);
            should_int((int)new_pcb->registros_cpu->CX) be equal to(0);
            should_int((int)new_pcb->registros_cpu->DX) be equal to(0);
            should_int((int)new_pcb->registros_cpu->EAX) be equal to(0);
            should_int((int)new_pcb->registros_cpu->EBX) be equal to(0);
            should_int((int)new_pcb->registros_cpu->ECX) be equal to(0);
            should_int((int)new_pcb->registros_cpu->EDX) be equal to(0);
            should_int((int)new_pcb->registros_cpu->SI) be equal to(0);
            should_int((int)new_pcb->registros_cpu->DI) be equal to(0);
            destroy_pcb(new_pcb);
        } end
    } end

    describe("Se hace update de los registros de CPU") {
        it("Debe crear un pcb con pid 0, quantum del config (el q esta acutalmente en el config) y todos los valores del registro de cpu en 0, y luego cambiar todos los valores del registro de cpu por 100") {
            gh_lpm();
            t_pcb* new_pcb = create_pcb();
            t_registros_cpu registro;
            t_contexto_ejecucion contexto;
            contexto.pid = 0;
            registro.PC = 100;
            registro.AX = 100;
            registro.BX = 100;
            registro.CX = 100;
            registro.DX = 100;
            registro.EAX = 100;
            registro.EBX = 100;
            registro.ECX = 100;
            registro.EDX = 100;
            registro.SI = 100;
            registro.DI = 100;
            contexto.registros_cpu = registro;
            should_int((int)new_pcb->pid) be equal to(0);
            should_int(update_pcb(new_pcb, contexto)) be equal to(0);
            should_int((int)new_pcb->registros_cpu->PC) be equal to(100);
            should_int((int)new_pcb->registros_cpu->AX) be equal to(100);
            should_int((int)new_pcb->registros_cpu->BX) be equal to(100);
            should_int((int)new_pcb->registros_cpu->CX) be equal to(100);
            should_int((int)new_pcb->registros_cpu->DX) be equal to(100);
            should_int((int)new_pcb->registros_cpu->EAX) be equal to(100);
            should_int((int)new_pcb->registros_cpu->EBX) be equal to(100);
            should_int((int)new_pcb->registros_cpu->ECX) be equal to(100);
            should_int((int)new_pcb->registros_cpu->EDX) be equal to(100);
            should_int((int)new_pcb->registros_cpu->SI) be equal to(100);
            should_int((int)new_pcb->registros_cpu->DI) be equal to(100);
            destroy_pcb(new_pcb);
        } end
    } end

    describe("Se hace update de los registros de CPU") {
        it("Debe fallar porque el pid es distinto") {
            gh_lpm();
            t_pcb* new_pcb = create_pcb();
            t_registros_cpu registro;
            t_contexto_ejecucion contexto;
            contexto.pid = 9;
            registro.PC = 100;
            registro.AX = 100;
            registro.BX = 100;
            registro.CX = 100;
            registro.DX = 100;
            registro.EAX = 100;
            registro.EBX = 100;
            registro.ECX = 100;
            registro.EDX = 100;
            registro.SI = 100;
            registro.DI = 100;
            contexto.registros_cpu = registro;
            should_int((int)new_pcb->pid) be equal to(0);
            should_int(update_pcb(new_pcb, contexto)) be equal to(1);
            destroy_pcb(new_pcb);
        } end
    } end

destroy_config_kernel();
destroy_logger();

}