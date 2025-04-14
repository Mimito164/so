#include "sockets.h"

int iniciar_servidor(t_log *logger, char *nombre_server, char *ip, char *puerto)
{
    int socket_servidor;

    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(ip, puerto, &hints, &servinfo);

    bool conecto = false;
    for (struct addrinfo *p = servinfo; p != NULL; p = p->ai_next)
    {
        socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

        // Setea una opción para reusar sockets "muertos" y que no quede en loop
        if (setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
        {
            log_error(logger, "setsockopt(SO_REUSEADDR) failed, al conectar (a %s)\n", nombre_server);
            freeaddrinfo(servinfo);
            return 0;
        }

        // fallo de crear socket
        if (socket_servidor == -1)
            continue;

        // fallo al hacer el bind
        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(socket_servidor);
            continue;
        }

        // creo el socket y bindeo correctamente
        conecto = true;
        break;
    }
    if (!conecto)
    {
        // free(servinfo);
        freeaddrinfo(servinfo);
        return 0;
    }

    listen(socket_servidor, SOMAXCONN);

    log_info(logger, "Escuchando en %s:%s (%s)\n", ip, puerto, nombre_server);

    freeaddrinfo(servinfo);

    return socket_servidor;
}

int esperar_cliente(t_log *logger, char *nombre_server, int socket_servidor)
{

    // Estas dos lineas entiendo que estan como placeholders para la funcion accept.
    struct sockaddr_in dir_cliente;
    socklen_t tam_direccion = sizeof(struct sockaddr_in);

    int socket_cliente = accept(socket_servidor, (void *)&dir_cliente, &tam_direccion);

    log_info(logger, "Se conecto un cliente (a %s)\n", nombre_server);

    return socket_cliente;
}

int crear_conexion(t_log *logger, char *nombre_server, char *ip, char *puerto)
{

    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(ip, puerto, &hints, &servinfo);

    int socket_cliente = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    if (socket_cliente == -1)
    {
        log_error(logger, "Error creando el socket para %s:%s", ip, puerto);
        return 0;
    }

    // Setea una opción para reusar sockets "muertos" y que no quede en loop
    if (setsockopt(socket_cliente, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    {
        log_error(logger, "setsockopt(SO_REUSEADDR) failed, al conectar (a %s)\n", nombre_server);
        freeaddrinfo(servinfo);
        return 0;
    }

    if (connect(socket_cliente, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        log_error(logger, "Error al conectar (a %s)\n", nombre_server);
        freeaddrinfo(servinfo);
        return 0;
    }

    log_info(logger, "Cliente conectado en %s:%s (a %s)\n", ip, puerto, nombre_server);

    freeaddrinfo(servinfo);

    return socket_cliente;
}

void liberar_conexion(int *socket_cliente)
{
    close(*socket_cliente);
    *socket_cliente = -1;
}

bool generar_conexion(t_log *logger, char *nombre_server, int *fd_server, char *ip, char *puerto)
{

    *fd_server = crear_conexion(logger, nombre_server, ip, puerto);

    return *fd_server != 0;
}
