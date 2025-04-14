#include "ciclo_instruccion/ciclo_instruccion.h"
#include <commons/string.h>
#include "comunicacion_kernel/contextoEjecucionHandlers/contextoEjecucionHandlers.h"

#include <cspecs/cspec.h>

context (ciclo_de_instruccion) {

    describe("Decode") {
        char** instruccion_decodificada;

        after {
            string_array_destroy(instruccion_decodificada);
        } end
        
        it("Deberia retornar un array con la instruccion y los argumentos") {
            char* instruccion = "SET AX 8";
            char* instruccion_esperada = "SET";
            char* argumentos_esperados = "AX 8";

            instruccion_decodificada = decode(instruccion);
            
            should_string(string_array_pop(instruccion_decodificada)) be equal to(argumentos_esperados);
            should_string(string_array_pop(instruccion_decodificada)) be equal to(instruccion_esperada);

        } end
    } end

    describe("Execute") {
        before {
            set_pc(9);
        }end

        it("Deberia ejecutar SET AX 4") {         
            char *data[] = {"SET","AX 4"};
            char** instruccion_decodificada = data;
            int valor_esperado = 4;
            int pc_esperado = 10;

            execute(instruccion_decodificada); 
            should_int(get_ax()) be equal to(valor_esperado);
            should_int(get_pc()) be equal to (pc_esperado);

        } end


    } end

    describe("Interrupt") {


        it("Deberia devolver el paquete con el contexto de ejecucion") {         
            int pid = 27;
            interrupt_type tipoInterrupcion = DESALOJO;
            int pc = 17;
            set_interrupt(pid, tipoInterrupcion);
            set_pid(pid);
            set_pc(pc);

            t_paquete* paquete = manejoDeInterrupciones(); 
            t_contexto_ejecucion contexto = deserializar_contexto_ejecucion(paquete);
            should_int(contexto.registros_cpu.PC) be equal to(pc);
            paquete_destroy(paquete);
        } end

        it("Deberia devolver NULL para que la ejecucion siga sin detenerse"){
            t_paquete* paquete = manejoDeInterrupciones();
            should_ptr(paquete) be equal to(NULL);
        } end
    } end
}