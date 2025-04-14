#include "user_mem/user_mem.h"
#include "serializacion/mensajes/cpu_memoria/msj_cpu_memoria.h"
#include "commons/collections/list.h"
#include <string.h>
#include <cspecs/cspec.h>

context(User_memory) {

    initialize_cfg("tests/testing.config"); // podemos tener configs diferentes para los tests
    instanciate_user_mem();

    describe("User memory"){

        it("should set and get values from memory"){

            uint32_t val = 3;
            uint32_t position = 0;
            uint32_t size = sizeof(val);

            set_val_to_usermem((void *)&val, position, size);

            uint32_t mem_val = *(uint32_t *)get_val_from_usermem(position, size);

            // ↑ no me gusta el casteo, pero lo hago solo en test
            // memoria no le importa el tipo, mete y saca lo que sea. No pregutna

            should_int(mem_val) be equal to(val);
        }end

    }end

    describe("get_val_from_usermem iterator"){

        it("should get big value from memory"){
            
            t_list * list = list_create();
            int size = 0; // 9
            for (int i = 0; i < 3; i++)
            {
                frames_element * pkt = malloc(sizeof(frames_element));
                pkt->pid = 1;
                pkt->offset = i == 0 ? 3 : 0;
                pkt->padding = i == 3 ? 1 : 0;
                pkt->frame = i;
                pkt->tam_pag = 4;
                list_add(list, pkt);
                size = size + (pkt->tam_pag - pkt->offset - pkt->padding);
            }

            char * val = "0123456789012345";
            // expected = ---345678901----
            //     offset ↑↑↑        ↑↑↑↑↑ no entra
            // en el primer frame metimos offset de 3
            // y luego el size es de 9 characters
            // por eso solo vamos a leer 34567890
            char* expect = "345678901";

            int position = 0;
            set_val_to_usermem((void *)val, position, strlen(val));
            char * data = get_val_from_usermem_iterator(list, size);
            
            for(int i = 0; i <= size; i++)
            {
                should_char(data[i]) be equal to (expect[i]);
            }

        }end

        it("should set big value into memory"){
            
            t_list * list = list_create();
            int size = 0; // 76
            for (int i = 0; i < 20; i++)
            {
                frames_element * pkt = malloc(sizeof(frames_element));
                pkt->pid = 1;
                pkt->offset = i == 0 ? 3 : 0;
                pkt->padding = i == 19 ? 1 : 0;
                pkt->frame = i + 4;
                pkt->tam_pag = 4;
                list_add(list, pkt);
                size = size + (pkt->tam_pag - pkt->offset - pkt->padding);
            }
            
            char * la_data = "Puta madre, este tp me va a matar si sigue así, es como si nunca fuera a terminar hasta volverme loco y relamnte destruir mi mente y mi estabilidad emocional";
            set_val_into_usermem_iterator(list, la_data);
            char * data = get_val_from_usermem_iterator(list, size);

            // printf("Size: %i \n", size);
            // printf("LA VERGA: %s \n", data);
            
            should_int(strlen(data)) be equal to(size);
            for (int i = 0; i <= size-1; i++) // size -1 pq data tiene \0 y la_data sigue
            {
                should_char(data[i]) be equal to(la_data[i]);
            }
            
        }end

    } end

    destroy_user_mem();
}