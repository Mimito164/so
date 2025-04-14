#include "instrucciones/instrucciones.h"
#include "serializacion/serializacion.h"
#include "serializacion/mensajes/cpu_kernel/msj_cpu_kernel.h"
#include "comunicacion_kernel/contextoEjecucionHandlers/contextoEjecucionHandlers.h"

#include <cspecs/cspec.h>

context (instrucciones) {

    describe("SET AX 3") {
    
        uint32_t pc = 0;

        before {
            set_pc(pc);
        } end
        
        it("Deberia AX valer 3") {
            char* argumentos = "AX 3";
            int valor_esperado = 3;
            int pc_esperado = 1;
            set(argumentos);
            should_int(get_ax()) be equal to(valor_esperado);
            should_int(get_pc()) be equal to (pc_esperado);
        } end
    } end

    describe("SUM AX BX") {
        
        uint32_t pc = 3;

        before {
            set_pc(pc);
            set_ax(2);
            set_bx(3);
        } end
        
        it("Deberia AX valer 5") {
            char* argumentos = "AX BX";
            int valor_esperado = 5;
            int pc_esperado = 4;
            sum(argumentos);
            should_int(get_ax()) be equal to(valor_esperado);
            should_int(get_pc()) be equal to (pc_esperado);
        } end
    } end

    describe("SUB AX BX") {

        uint32_t pc = 7;
        
        before {
            set_pc(pc);
            set_ax(2);
            set_bx(3);
        } end
        
        it("Deberia BX valer 1") {
            char* argumentos = "BX AX";
            int valor_esperado = 1;
            int pc_esperado = 8;
            sub(argumentos);
            should_int(get_bx()) be equal to(valor_esperado);
            should_int(get_pc()) be equal to (pc_esperado);
        } end
    } end

    describe("JNZ AX 4") {

        before {
            set_pc(0);
            set_ax(2);
        } end
        
        it("Deberia PC valer 4") {
            char* argumentos = "AX 4";
            int valor_esperado = 4;
            jnz(argumentos);
            should_int(get_pc()) be equal to(valor_esperado);
        } end
    } end

    describe("JNZ AX 4") {
        before {
            set_pc(0);
            set_ax(0);
        } end
        
        it("Deberia PC valer 1") {
            char* argumentos = "AX 4";
            int valor_esperado = 1;
            jnz(argumentos);
            should_int(get_pc()) be equal to(valor_esperado);
        } end
    } end

    describe("IO_GEN_SLEEP impresoraDeDiego 8"){

        before {
            set_pc(0);
        } end

        it("Deberia serializar bien el paquete"){
            char* argumentos= "impresoraDeDiego 8";
            char* nombreIO = "impresoraDeDiego";
            int unidadesDeTrabajoEsperadas = 8;
            int PCEsperado = 1;
            t_paquete* paquete = gen_io_sleep(argumentos);
            s_cpu_kernel estructura = deserializar_cpu_kernel_io_sleep(paquete);
            should_string(estructura.solicitud_recurso.args_io_sleep.nombre_io) be equal to (nombreIO);
            should_int(estructura.solicitud_recurso.args_io_sleep.unidades_trabajo) be equal to (unidadesDeTrabajoEsperadas);
            should_int(estructura.contexto.registros_cpu.PC) be equal to (PCEsperado);
        } end

    } end

    describe("IO_FS_DELETE"){

        before {
            set_pc(0);
        } end

         it("Deberia serializar bien el paquete"){
            char* argumentos= "int4 notas.txt";
            char* interfaz = "int4";
            char* nombre_archivo= "notas.txt";
            int PCEsperado = 1;

            t_paquete* paquete = io_fs_delete(argumentos);
            s_cpu_kernel estructura = deserializar_cpu_kernel_io_fs_delete(paquete);
            should_string(estructura.solicitud_recurso.args_io_fs_delete.nombre_io) be equal to (interfaz);
            should_string(estructura.solicitud_recurso.args_io_fs_delete.nombre_archivo) be equal to (nombre_archivo);
            should_int(estructura.contexto.registros_cpu.PC) be equal to (PCEsperado);
         }end

    }end

    describe("IO_FS_CREATE"){

        before {
            set_pc(0);
        } end

        it("Deberia serializar bien el paquete"){
            char* argumentos= "int4 notas.txt";
            char* interfaz = "int4";
            char* nombre_archivo= "notas.txt";
            int PCEsperado = 1;
           
            t_paquete* paquete = io_fs_create(argumentos);
            s_cpu_kernel estructura = deserializar_cpu_kernel_io_fs_create(paquete);
            should_string(estructura.solicitud_recurso.args_io_fs_create.nombre_io) be equal to (interfaz);
            should_string(estructura.solicitud_recurso.args_io_fs_create.nombre_archivo) be equal to (nombre_archivo);
            should_int(estructura.contexto.registros_cpu.PC) be equal to (PCEsperado);
        } end

    } end

    describe("IO_FS_TRUNCATE"){

        before {
            set_pc(0);
        } end

        it("Deberia serializar bien el paquete"){
            set_ax(64);
            char* argumentos= "int4 notas.txt AX";
            char* interfaz = "int4";
            char* nombre_archivo= "notas.txt";
            int tamanio = 64;
            int PCEsperado = 1;

            t_paquete* paquete = io_fs_truncate(argumentos);
            s_cpu_kernel estructura = deserialziar_cpu_kernel_io_fs_truncate(paquete);
            should_string(estructura.solicitud_recurso.args_io_fs_truncate.nombre_io) be equal to (interfaz);
            should_string(estructura.solicitud_recurso.args_io_fs_truncate.nombre_archivo) be equal to (nombre_archivo);
            should_int(estructura.solicitud_recurso.args_io_fs_truncate.tamano) be equal to (tamanio);
            should_int(estructura.contexto.registros_cpu.PC) be equal to (PCEsperado);
        } end
    }end
}