#include "mmu.h"

// uint32_t obtener_pagina(uint32_t direccion_logica)
// {
//     // NOLINTNEXTLINE(bugprone-integer-division)
//     return floor(direccion_logica / get_tam_pag());
// }

// //  NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
// uint32_t obtener_desplazamiento(uint32_t direccion_logica, uint32_t pagina)
// {
//     // dirección_lógica - número_página * tamaño_página
//     return direccion_logica - (pagina * get_tam_pag());
// }

//  NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
t_list *traducir_memoria(uint32_t direccion_logica, uint32_t size, uint32_t pid)
{
    t_list *result = list_create();

#ifndef TEST
    uint32_t tam_pag = get_tam_pag();
#else
    uint32_t tam_pag = 4;
#endif

    // Consigna: número_página = floor(dirección_lógica / tamaño_página)
    uint32_t pagina = floor(direccion_logica / tam_pag); // NOLINT(bugprone-integer-division)
    // Consigna: offset = dirección_lógica - número_página * tamaño_página
    uint32_t offset = direccion_logica - (pagina * tam_pag);

    uint32_t indice_pagina = 0;
    int left_to_save = (int)size;

    while (left_to_save >= 1)
    {
        frames_element *frames_element_pkt = malloc(sizeof(frames_element));
        frames_element_pkt->pid = pid;
        frames_element_pkt->offset = offset;
        frames_element_pkt->padding = left_to_save < tam_pag - offset ? (tam_pag - left_to_save - offset) : 0;
        frames_element_pkt->tam_pag = tam_pag;

        left_to_save -= (int)(tam_pag - offset);
        offset = 0;

        int frame = obtener_entrada((int)(pagina + indice_pagina), (int)pid);

#ifndef TEST
        if (frame == -1) // tlb miss
        {
            log_info(get_logger(), "PID: <%d> - TLB MISS - Pagina: <%d>", pid, pagina + indice_pagina);
            frame = (int)obtener_frame(pid, pagina + indice_pagina); // le pido a memoria osea un send
            // TODO(all): que pasa si obtener frame no consigue el marco
            cargar_si_puedo(frame, (int)(pagina + indice_pagina), (int)pid);
        }
        else
        {
            log_info(get_logger(), "PID: <%d> - TLB HIT - Pagina: <%d>", pid, pagina + indice_pagina);
        }
#endif
        frames_element_pkt->frame = frame;

        list_add(result, (void *)frames_element_pkt);

        indice_pagina++;
    }

    return result;
}