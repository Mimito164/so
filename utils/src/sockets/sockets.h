#ifndef SOCKETS_H_
#define SOCKETS_H_

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <commons/log.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

// contenido util:
// https://beej.us/guide/bgnet/html/split/system-calls-or-bust.html#system-calls-or-bust

// TODO: Mejorar estos comentarios para que se vea mas faxero en el vs con el
// intellisense (la wea que muestra la descripcion cuando haces hover).

/**
 * @name: iniciar_servidor
 * @DESC: Es una funcion de un servidor para iniciase
 * @param:
 *  logger: logger pa loguear weas
 *  nombre_server: es el nombre del sercvidor que intenta iniciarse, se usa en
 * los logs del logger ip: lo usa getaddrinfo() puerto: idem anterior
 * @return: retorna el socket de escucha del servidor o 0 si no pudo conectar
 */

int iniciar_servidor(t_log *logger, char *nombre_server, char *ip,
                     char *puerto);

/**
 * @name: esperar_cliente
 * @DESC: Es una funcion del servidor. Retorna un nuevo socket (file descriptor)
 * que representa la conexión BIDIRECCIONAL entre el servidor y el cliente
 * socket
 * @param:
 *  logger: logger pa loguear weas
 *  nombre_server: es el nombre del servidor al cual el cliente quiere
 * conectarse, se usa en los logs del logger socket_servidor:  es el socket del
 * servidor al cual el cliente quiere conectarse, lo usa accept()
 * @return: -1 si no se pudo conectar, o el socket del cliente (file descriptor)
 */

int esperar_cliente(t_log *logger, char *nombre_server, int socket_servidor);

/**
 * @name: crear_conexion
 * @DESC: Es una funcion del Cliente, crea el socket del cliente y lo conecta
 * con el servidor previamente inicializado.
 * @param:
 *  logger: logger pa loguear weas
 *  nombre_server: es un nombre para que aparezca en el logger a que server
 * corresponden los logs socket_servidor: lo usa accept()
 * @return: 0 si no se pudo conectar, o el socket del cliente (file descriptor)
 */

int crear_conexion(t_log *logger, char *nombre_server, char *ip, char *puerto);

/**
 * @name: liberar_conexion
 * @DESC: Es una funcion del Servidor, cierra el socket del cliente
 * @param:
 *  socket_cliente: el socket del cliente...
 * @return: void
 */

void liberar_conexion(int *socket_cliente);


/**
 * @name: generar_conexion
 * @DESC: Es una funcion del Cliente, para crear una conexion con un server
 * @param:
 *  logger: logger pa loguear weas
 *  nombre_server: una string que guarde el nombre del server el cual sera logueado cuando se haya conectado.
 *  fd_server: fd del server con el que nos queremos conectar
 *  ip: string que contenga la ip del server con el que nos queremos conectar
 *  puerto: string que contenga el puerto del server con el que nos queremos conectar
 * @return: true, si logro generar la conexion. false, si no logro generar la conexion. 
 */

bool generar_conexion(t_log *logger, char* nombre_server, int *fd_server, char* ip, char* puerto);

#endif // SOCKETS_H_