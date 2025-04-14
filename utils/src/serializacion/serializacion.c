#include "serializacion.h"

t_paquete *paquete_new()
{
    t_paquete *paquete = malloc(sizeof(t_paquete));
    paquete->buffer = malloc(sizeof(t_buffer));
    paquete->buffer->size = 0;
    paquete->buffer->stream = NULL;

    /* no hago un malloc para el stream porque de eso
     * se encargan las funciones para serializar */
    return paquete;
}

void paquete_destroy(t_paquete *paquete)
{
    if (paquete == NULL)
    {
        printf("El paquete es nulo\n");
        return;
    }

    if (paquete->buffer == NULL)
    {
        printf("El buffer es nulo\n");
        return;
    }

    if (paquete->buffer->size > 0)
    {
        //     free(paquete->buffer->stream);
        free(paquete->buffer->stream_inicial);
    }

    // paquete->buffer->stream_inicial = NULL;
    free(paquete->buffer);
    paquete->buffer = NULL;
    free(paquete);
    paquete = NULL;
}

bool recv_paquete(int file_descriptor, t_paquete **paquete)
{
    *paquete = malloc(sizeof(t_paquete));
    (*paquete)->buffer = malloc(sizeof(t_buffer));

    int recv_status;
    // NOLINTNEXTLINE(bugprone-narrowing-conversions)
    recv_status = recv(file_descriptor, &(*paquete)->codigo_operacion, sizeof(op_code), MSG_WAITALL);

    // nos envió algo sin un op_code, osea basura
    if (recv_status != sizeof(op_code))
    {
        free((*paquete)->buffer);
        free(*paquete);
        return false;
    }

    // esto significa que se desconectó
    if (recv(file_descriptor, &(*paquete)->buffer->size, sizeof(uint32_t), 0) == -1)
    {
        free((*paquete)->buffer);
        (*paquete)->buffer = NULL;
        free(*paquete);
        *paquete = NULL;
        return false;
    }

    if ((*paquete)->buffer->size != 0)
    {
        (*paquete)->buffer->stream = malloc((*paquete)->buffer->size);
        (*paquete)->buffer->stream_inicial = (*paquete)->buffer->stream;

        if (recv(file_descriptor, (*paquete)->buffer->stream, (*paquete)->buffer->size, 0) == -1)
        {
            paquete_destroy(*paquete);
            return false;
        }
    }

    return true;
}

bool send_paquete(int file_descriptor, t_paquete *paquete)
{
    int offset = 0;
    uint32_t size_paquete_serializado = sizeof(op_code) + sizeof(uint32_t) + paquete->buffer->size;
    void *paquete_serializado = malloc(size_paquete_serializado);

    memcpy(paquete_serializado + offset, &paquete->codigo_operacion, sizeof(op_code));
    offset += sizeof(op_code);
    memcpy(paquete_serializado + offset, &paquete->buffer->size, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    if (paquete->buffer->size != 0)
    {
        memcpy(paquete_serializado + offset, paquete->buffer->stream, paquete->buffer->size);
    }

    // ↓↓↓ Importante: MSG_NOSIGNAL hace que si send falla (pq del otro lado se desconectaron) no corte la ejecución
    if (send(file_descriptor, paquete_serializado, size_paquete_serializado, MSG_NOSIGNAL) == -1)
    {

        // nos olvidamos de cerrar el fd close(file_descriptor)
        free(paquete_serializado);
        return false;
    }
    // printf("%d",p->codigo_operacion);
    free(paquete_serializado);
    if (paquete->buffer->stream != NULL)
    {
        free(paquete->buffer->stream);
    }
    free(paquete->buffer);
    free(paquete);

    return true;
}

void serializar_uint8_t(t_buffer **buffer, uint8_t int8)
{
    if ((*buffer)->size == 0)
    {
        (*buffer)->stream = malloc(sizeof(uint8_t));
        (*buffer)->stream_inicial = (*buffer)->stream;
    }

    else
    {
        (*buffer)->stream = realloc((*buffer)->stream, (*buffer)->size + sizeof(uint8_t));
        (*buffer)->stream_inicial = (*buffer)->stream;
    }

    memcpy((*buffer)->stream + (*buffer)->size, &int8, sizeof(uint8_t));
    (*buffer)->size += sizeof(uint8_t);
}

void serializar_uint16_t(t_buffer **buffer, uint16_t int16)
{
    if ((*buffer)->size == 0)
    {
        (*buffer)->stream = malloc(sizeof(uint16_t));
        (*buffer)->stream_inicial = (*buffer)->stream;
    }
    else
    {
        (*buffer)->stream = realloc((*buffer)->stream, (*buffer)->size + sizeof(uint16_t));
        (*buffer)->stream_inicial = (*buffer)->stream;
    }

    memcpy((*buffer)->stream + (*buffer)->size, &int16, sizeof(uint16_t));
    (*buffer)->size += sizeof(uint16_t);
}

void serializar_uint32_t(t_buffer **buffer, uint32_t int32)
{
    if ((*buffer)->size == 0)
    {
        (*buffer)->stream = malloc(sizeof(uint32_t));
        (*buffer)->stream_inicial = (*buffer)->stream;
    }

    else
    {
        (*buffer)->stream = realloc((*buffer)->stream, (*buffer)->size + sizeof(uint32_t));
        (*buffer)->stream_inicial = (*buffer)->stream;
    }

    memcpy((*buffer)->stream + (*buffer)->size, &int32, sizeof(uint32_t));
    (*buffer)->size += sizeof(uint32_t);
}

void serializar_string(t_buffer **buffer, char *string)
{
    uint32_t str_length = string_length(string) + 1;
    serializar_uint32_t(buffer, str_length);

    if ((*buffer)->size == 0)
    {
        (*buffer)->stream = malloc(str_length);
        (*buffer)->stream_inicial = (*buffer)->stream;
    }

    else
    {
        (*buffer)->stream = realloc((*buffer)->stream, (*buffer)->size + str_length);
        (*buffer)->stream_inicial = (*buffer)->stream;
    }

    memcpy((*buffer)->stream + (*buffer)->size, string, str_length);
    (*buffer)->size += str_length;
}

void deserializar_uint8_t(t_buffer *buffer, uint8_t *int8)
{
    if (buffer->size == 0)
    {
        printf("El stream esta vacio.\n");
        return;
    }
    if (buffer->size < sizeof(uint8_t))
    {
        printf("Estas deserializando mas bytes de los que hay en el stream.\n");
        return;
    }
    memcpy(int8, buffer->stream, sizeof(uint8_t));
    buffer->stream += sizeof(uint8_t);
    buffer->size -= sizeof(uint8_t);
}

void deserializar_uint16_t(t_buffer *buffer, uint16_t *int16)
{
    if (buffer->size == 0)
    {
        printf("El stream esta vacio.\n");
        return;
    }
    if (buffer->size < sizeof(uint16_t))
    {
        printf("Estas deserializando mas bytes de los que hay en el stream.\n");
        return;
    }
    memcpy(int16, buffer->stream, sizeof(uint16_t));
    buffer->stream += sizeof(uint16_t);
    buffer->size -= sizeof(uint16_t);
}

void deserializar_uint32_t(t_buffer *buffer, uint32_t *int32)
{
    if (buffer->size == 0)
    {
        printf("El stream esta vacio.\n");
        return;
    }
    if (buffer->size < sizeof(uint32_t))
    {
        printf("Estas deserializando mas bytes de los que hay en el stream.\n");
        return;
    }
    memcpy(int32, buffer->stream, sizeof(uint32_t));
    buffer->stream += sizeof(uint32_t);
    buffer->size -= sizeof(uint32_t);
}

void deserializar_string(t_buffer *buffer, char **string)
{
    if (buffer->size == 0)
    {
        printf("El stream esta vacio.\n");
        return;
    }

    uint32_t str_length;
    deserializar_uint32_t(buffer, &str_length);
    // NOLINTNEXTLINE(clang-analyzer-core.UndefinedBinaryOperatorResult)
    if (buffer->size < str_length)
    {
        printf("Estas deserializando mas bytes de los que hay en el stream.\n");
        return;
    }

    *string = malloc(str_length);
    memcpy(*string, buffer->stream, str_length);

    buffer->stream += str_length;
    buffer->size -= str_length;
}

/// @brief Serializa una lista usando una funcion seializadora.
/// @param buffer referencia del buffer del paquete
/// @param list la lista donde se leen los datos
/// @param deserializador funcion que serializa un elemento de la lista
/// @warning el `serializador` DEBE tener el siguiente formato `void nombre(t_buffer**, t_elemento_lista*)` donde toma
/// la referencia del buffer del paquete y un puntero de un elemento de la lista
void serializar_t_list(t_buffer **buffer, t_list *list, void (*serializador)(void **, void *))
{
    serializar_uint32_t(&(*buffer), (uint32_t)list_size(list));

    t_list_iterator *iterator = list_iterator_create(list);

    while (list_iterator_has_next(iterator))
    {
        serializador((void **)buffer, list_iterator_next(iterator));
    }

    list_iterator_destroy(iterator);
}

/// @brief Deserializa una lista usando una funcion deserializadora.
/// @param buffer buffer del paquete
/// @param list REFERENCIA a la lista donde se guardaran los datos
/// @param deserializador funcion que deserializa un elemento de la lista
/// @warning el `deserializador` DEBE tener el siguiente formato `t_elemento_lista* nombre(t_buffer*)` donde toma el
/// buffer del paquete y retorna un puntero a la variable de la lista
void deserializar_t_list(t_buffer *buffer, t_list **list, void *(*deserializador)(t_buffer *))
{
    uint32_t largo;
    deserializar_uint32_t(buffer, &largo);

    // NOLINTNEXTLINE(clang-analyzer-core.UndefinedBinaryOperatorResult)
    for (uint32_t i = 0; i < largo; i++)
    {
        list_add(*list, deserializador(buffer));
    }
}
