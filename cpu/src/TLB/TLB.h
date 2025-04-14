#ifndef TLB_H_
#define TLB_H_

#include <commons/collections/list.h>
#include <commons/temporal.h>
#include <globals/globals.h>
#include <stdint.h>
#include <string.h>
#include <t_config/t_config.h>

typedef struct
{
    int n_pag;
    int n_marco;
    int pid;
    t_temporal *timestamp;

} t_entrada;

/// --------------------------- TLB ----------------------------
void init_TLB();
t_entrada *crear_entrada(int marco, int pag, int pid);
void cargar_entrada(int marco, int pag, int pid);
void reemplazar_entrada();
void actualizar_entrada(t_entrada *entrada_a_matar);
int obtener_tamanio_tlb();
int obtener_entrada(int pagina, int pid);
void cargar_si_puedo(int marco, int pag, int pid);
void reemplazar_por_LRU(int marco, int pag, int pid);
void reemplazar_por_fifo(int marco, int pag, int pid);
void destroy_tlb();

#endif