#ifndef BITMAP_FRAMES_LIBRES_H_
#define BITMAP_FRAMES_LIBRES_H_

#include <commons/bitarray.h>
#include <semaphore.h>
#include <t_config/t_config.h>

void crear_bitmap_frames_libres();
void destroy_bitmap_frames_libres();
bool esFrameLibre(int indice);
void asignarFrame(int indice);
bool asignarFrames(int cantidad, t_list *frames_tabla);
void liberar_frames(int cantidad, t_list *frames_tabla);
void liberar_frame(int index);

// pa tests
t_bitarray *get_bitmap();

#endif