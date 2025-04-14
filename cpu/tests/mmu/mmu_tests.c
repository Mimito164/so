#include "mmu/mmu.h"
#include "TLB/TLB.h"
#include <cspecs/cspec.h>

context(MMU){
    describe("La MMU") {
        
        initialize_cfg("cpu.config");
        before {
            init_TLB();
        } end

        after{
            destroy_tlb();
        } end

        it("debería poder traducir varias ") {

            uint32_t direc_logica = 21;
            uint32_t size = 12; 
            uint32_t pid = 1;
            uint32_t tam_pag = 4; // → esto está harcodeado en traducir memoria

            // Consigna: número_página = floor(dirección_lógica / tamaño_página)
            uint32_t pagina = floor(direc_logica / tam_pag); // NOLINT(bugprone-integer-division)
            // Consigna: offset = dirección_lógica - número_página * tamaño_página
            uint32_t offset = direc_logica - (pagina * tam_pag);
            uint32_t padding = tam_pag - ((size - (tam_pag - offset)) % tam_pag);

            for (int i = 0; i <= 10; ++i) {
                // cargar_entrada(marco, pagina, pid);
                cargar_entrada((i+1),i,pid);
            }

            t_list * result = traducir_memoria(direc_logica, size, pid);
            int resutl_size = list_size(result);
            for (int i = 0; i < resutl_size; i++)
            {

                frames_element * frames_element_pkt = (frames_element *) list_get(result, i);

                should_int(frames_element_pkt->pid) be equal to(pid);
                should_int(frames_element_pkt->tam_pag) be equal to(tam_pag);      

                int frame_expected = pagina + i + 1;
                should_int(frames_element_pkt->frame) be equal to(frame_expected);

                if (i == 0) {
                    should_int(frames_element_pkt->offset) be equal to(offset);
                } else {
                    should_int(frames_element_pkt->offset) be equal to(0);
                }

                if (i == resutl_size-1) {
                    should_int(frames_element_pkt->padding) be equal to(padding);
                } else {
                    should_int(frames_element_pkt->padding) be equal to(0);
                }

            }
        }end
    }end 
}