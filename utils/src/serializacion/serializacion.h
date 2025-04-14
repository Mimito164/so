#ifndef SERIALIZACION_H_
#define SERIALIZACION_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include <commons/string.h>

#include <TAD/TAD.h>

typedef struct
{
    uint32_t size;        // Tamaño del payload
    void *stream;         // Payload
    void *stream_inicial; // Referencia del inicio
} t_buffer;

typedef struct
{
    op_code codigo_operacion;
    t_buffer *buffer;
} t_paquete;

t_paquete *paquete_new(void);
void paquete_destroy(t_paquete *paquete);
/* recibe un fd para hacer los recive y un puntero a un paquete
 * forma de invocar:
 *      t_paquete* mi_paquete; // es un paquete nuevo
 *      recv_paquete(fd_memoria, &mi_paquete);
 */

bool recv_paquete(int file_descriptor, t_paquete **paquete);

bool send_paquete(int file_descriptor, t_paquete *paquete);

void serializar_uint8_t(t_buffer **buffer, uint8_t int8);
void serializar_uint16_t(t_buffer **buffer, uint16_t int16);
void serializar_uint32_t(t_buffer **buffer, uint32_t int32);
void serializar_string(t_buffer **buffer, char *string);

void serializar_t_list(t_buffer **buffer, t_list *list, void (*serializador)(void **, void *));

void deserializar_uint8_t(t_buffer *buffer, uint8_t *int8);
void deserializar_uint16_t(t_buffer *buffer, uint16_t *int16);
void deserializar_uint32_t(t_buffer *buffer, uint32_t *int32);
void deserializar_string(t_buffer *buffer, char **string);

void deserializar_t_list(t_buffer *buffer, t_list **list, void *(*deserializador)(t_buffer *));

#endif