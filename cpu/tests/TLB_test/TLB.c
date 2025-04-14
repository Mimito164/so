#include"../src/TLB/TLB.h"
#include"../../src/t_config/t_config.h"
#include <cspecs/cspec.h>

context(TLB){
             initialize_cfg("tests/testing.config");
    describe("_-----TLB-------") {

          before {
            init_TLB();
        } end
        after{
            destroy_tlb();
        } end

        it("Se pruebo la creacion / carga y entrada TLB") {
            cargar_entrada(0,1,2);
            int tamanio_tlb = obtener_tamanio_tlb();
            should_int(tamanio_tlb) be equal to(1);
        } end

        it("Deberia cargar la nueva pagina con fifo") {
            cargar_entrada(0,1,2); 
            cargar_entrada(1,2,3); 
            reemplazar_por_fifo(4,4,4);
            int entrada_obtenida = obtener_entrada(4,4);
            should_int(entrada_obtenida) be equal to(4);
            should_int(obtener_entrada(1,2)) be equal to(-1);
        } end

        it("Deberia cargar la nueva pagina con LRU") {
            cargar_entrada(0,1,2); 
            cargar_entrada(1,2,3); 
            obtener_entrada(2,3);
            reemplazar_por_LRU(0,3,4);
            int entrada_obtenida = obtener_entrada(3,4);
            should_int(entrada_obtenida) be equal to(0);
            
        } end

        it("LRU al fallo ") {
            cargar_entrada(0,1,2); 
            int entrada_obtenida = obtener_entrada(3,4);
            should_int(entrada_obtenida) be equal to(-1);
        } end
        
        it("Deberia cargar la nueva pagina con LRU y cambiar el (1,2,3)") {
            cargar_entrada(0,4,7); 
            cargar_entrada(1,2,3); 
            obtener_entrada(4,7);
            reemplazar_por_LRU(1,3,4);
            should_int(obtener_entrada(3,4)) be equal to(1);
            should_int(obtener_entrada(2,3)) be equal to(-1);
        } end
        
        it("Deberia cargar la nueva pagina") {
            // cargar_entrada(1,2,3); 
            cargar_si_puedo(0,1,2);
            int entrada_obtenida = obtener_entrada(1,2);
            should_int(entrada_obtenida) be equal to(0);
        } end
    } end
            destroy_config_cpu();
}