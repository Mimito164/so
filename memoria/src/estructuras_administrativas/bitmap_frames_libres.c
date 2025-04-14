#include "bitmap_frames_libres.h"

typedef struct t_bitmap_frames_libres
{
    t_bitarray *bitmap_frames_libres;
    sem_t mutex;

} t_bitmap_frames_libres;

static t_bitmap_frames_libres *bitmap;

static int obtener_cant_frames()
{
    int tam_mem = get_config()->TAM_MEMORIA;
    int tam_pag = get_config()->TAM_PAGINA;

    return tam_mem / tam_pag;
}

void crear_bitmap_frames_libres()
{
    // Esta expresado en bytes
    int cant_frames = obtener_cant_frames();
    char *bitarray = (char *)malloc((cant_frames) * sizeof(char));
    memset(bitarray, '\0', (cant_frames));

    bitmap = malloc(sizeof(t_bitmap_frames_libres));
    sem_init(&(bitmap->mutex), 0, 1);
    bitmap->bitmap_frames_libres = bitarray_create_with_mode(bitarray, cant_frames, LSB_FIRST);
}

void destroy_bitmap_frames_libres()
{
    bitarray_destroy(bitmap->bitmap_frames_libres);
    bitmap->bitmap_frames_libres = NULL;
    sem_destroy(&(bitmap->mutex));
    free(bitmap);
    bitmap = NULL;
}

bool esFrameLibre(int indice)
{
    // sem_wait(&(bitmap->mutex));
    bool respuesta = !bitarray_test_bit(bitmap->bitmap_frames_libres, indice);
    // sem_post(&(bitmap->mutex));
    return respuesta;
}

bool asignarFrames(int cantidad, t_list *frames_tabla)
{
    t_list *frames = list_create();
    sem_wait(&(bitmap->mutex));
    // recorro el bitmap
    for (int i = 0; i < bitarray_get_max_bit(bitmap->bitmap_frames_libres) / 8; i++)
    {
        if (esFrameLibre(i))
        {
            // si esta libre lo agrego a la lista
            list_add(frames, (void *)i);
            // si ya encontre la cantidad q necesito, asigno los frames
            if (list_size(frames) == cantidad)
            {
                for (int j = 0; j < cantidad; j++)
                {
                    asignarFrame((int)list_get(frames, j));
                    list_add(frames_tabla, (void *)list_get(frames, j));
                }
                list_destroy(frames);
                sem_post(&(bitmap->mutex));
                return true;
            }
        }
    }
    list_destroy(frames);
    sem_post(&(bitmap->mutex));
    return false;
}

void liberar_frame(int index)
{
    bitarray_clean_bit(bitmap->bitmap_frames_libres, index);
}

void liberar_frames(int cantidad, t_list *frames_tabla)
{
    sem_wait(&(bitmap->mutex));
    for (int i = 0; i < cantidad; i++)
    {
        int frame = (int)list_remove(frames_tabla, list_size(frames_tabla) - 1);
        liberar_frame(frame);
    }
    sem_post(&(bitmap->mutex));
}

void asignarFrame(int indice)
{
    // sem_wait(&(bitmap->mutex));
    bitarray_set_bit(bitmap->bitmap_frames_libres, indice);
    // sem_post(&(bitmap->mutex));
}

// pa tests
t_bitarray *get_bitmap()
{
    return bitmap->bitmap_frames_libres;
}