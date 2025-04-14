#include "iniciar_comunicacion.h"
#include "globals/globals.h"

int fd_kernel;

static void send_io_kernel_INICIAR_CONEXION();
static void delegante(t_paquete *paquete_reciv);

void iniciar_comunicacion_kernel()
{
    t_log *logger = get_logger();
    t_config_io *cfg = get_config();

    if (!generar_conexion(logger, "Server kernel", &fd_kernel, cfg->IP_KERNEL, cfg->PUERTO_KERNEL))
    {
        mem_clean_exit();
    }
    send_io_kernel_INICIAR_CONEXION();
    log_info(logger, "Se envio la presentacion a KERNEL");

    t_paquete *paquete_a_recibir = paquete_new();
    while (recv_paquete(fd_kernel, &paquete_a_recibir))
    {
        delegante(paquete_a_recibir);
    }
    // paquete_destroy(paquete_a_recibir); aca no se conviene destruir el paquete porque si salio del while es porque no
    // hay paquete xd lol
    liberar_conexion(&fd_kernel);
}

static void send_io_kernel_INICIAR_CONEXION()
{
    t_log *logger = get_logger();
    t_config_io *config = get_config();

    s_io_kernel_iniciar_conexion s_io_kernel_init_conec = {.nombreDeIO = get_nombre_io(),
                                                           .tipo = config->TIPO_INTERFAZ};
    t_paquete *paquete_a_enviar = serializar_io_kernel_INICIAR_CONEXION(s_io_kernel_init_conec);

    if (!send_paquete(fd_kernel, paquete_a_enviar))
    {
        // log_error(logger, "Paquete falluto \n");
        mem_clean_exit();
    }
}

static void delegante(t_paquete *paquete_reciv)
{
    t_log *logger = get_logger();
    t_config_io *cfg = get_config();
    switch (paquete_reciv->codigo_operacion)
    {
    case KERNEL_IO_FS_CREATE: {
        log_info(logger, "Kernel nos envio - KERNEL_IO_FS_CREATE");
        if (cfg->TIPO_INTERFAZ != DialFS)
        {
            // log_error(logger, "KERNEL NOS ENVIO UNA OPERACION PARA UNA DialFS");
            mem_clean_exit();
        }
        t_paquete *paquete_send = resolver_KERNEL_IO_FS_CREATE(paquete_reciv);
        send_paquete(fd_kernel, paquete_send);
        log_info(logger, "Se envia a Kernel - IO_KERNEL_FS_CREATE");
        break;
    }
    case KERNEL_IO_FS_DELETE: {
        log_info(logger, "Kernel nos envio - KERNEL_IO_FS_DELETE");
        if (cfg->TIPO_INTERFAZ != DialFS)
        {
            // log_error(logger, "KERNEL NOS ENVIO UNA OPERACION PARA UNA DialFS");
            mem_clean_exit();
        }
        t_paquete *paquete_send = resolver_KERNEL_IO_FS_DELETE(paquete_reciv);
        send_paquete(fd_kernel, paquete_send);
        log_info(logger, "Se envia a Kernel - IO_KERNEL_FS_DELETE");
        break;
    }
    case KERNEL_IO_FS_READ: {
        log_info(logger, "Kernel nos envio - KERNEL_IO_FS_READ");
        if (cfg->TIPO_INTERFAZ != DialFS)
        {
            // log_error(logger, "KERNEL NOS ENVIO UNA OPERACION PARA UNA DialFS");
            mem_clean_exit();
        }
        t_paquete *paquete_send = resolver_KERNEL_IO_FS_READ(paquete_reciv);
        send_paquete(fd_kernel, paquete_send);
        log_info(logger, "Se envia a Kernel - IO_KERNEL_FS_READ");
        break;
    }
    case KERNEL_IO_FS_TRUNCATE: {
        log_info(logger, "Kernel nos envio - KERNEL_IO_FS_TRUNCATE");
        if (cfg->TIPO_INTERFAZ != DialFS)
        {
            // log_error(logger, "KERNEL NOS ENVIO UNA OPERACION PARA UNA DialFS");
            mem_clean_exit();
        }
        t_paquete *paquete_send = resolver_KERNEL_IO_FS_TRUNCATE(paquete_reciv);
        send_paquete(fd_kernel, paquete_send);
        log_info(logger, "Se envia a Kernel - IO_KERNEL_FS_TRUNCATE");
        break;
    }
    case KERNEL_IO_FS_WRITE: {
        log_info(logger, "Kernel nos envio - KERNEL_IO_FS_WRITE");
        if (cfg->TIPO_INTERFAZ != DialFS)
        {
            // log_error(logger, "KERNEL NOS ENVIO UNA OPERACION PARA UNA DialFS");
            mem_clean_exit();
        }
        t_paquete *paquete_send = resolver_KERNEL_IO_FS_WRITE(paquete_reciv);
        send_paquete(fd_kernel, paquete_send);
        log_info(logger, "Se envia a Kernel - IO_KERNEL_FS_WRITE");
        break;
    }
    case KERNEL_IO_GEN_ZZZ: {
        log_info(logger, "Kernel nos envio - KERNEL_IO_GEN_ZZZ");
        if (cfg->TIPO_INTERFAZ != GENERICA)
        {
            // log_error(logger, "KERNEL NOS ENVIO UNA OPERACION PARA UNA IO GENERICA");
            mem_clean_exit();
        }
        t_paquete *paquete_send = resolver_KERNEL_IO_GEN_ZZZ(paquete_reciv);
        send_paquete(fd_kernel, paquete_send);
        log_info(logger, "Se envia a Kernel - IO_KERNEL_GEN_ZZZ");
        break;
    }
    case KERNEL_IO_STDIN_READ: {
        log_info(logger, "Kernel nos envio - KERNEL_IO_STDIN_READ");
        if (cfg->TIPO_INTERFAZ != STDIN)
        {
            // log_error(logger, "KERNEL NOS ENVIO UNA OPERACION PARA UNA IO STDIN");
            mem_clean_exit();
        }
        t_paquete *paquete_send = resolver_KERNEL_IO_STDIN_READ(paquete_reciv);
        send_paquete(fd_kernel, paquete_send);
        log_info(logger, "Se envia a Kernel - IO_KERNEL_STDIN_READ");
        break;
    }
    case KERNEL_IO_STDOUT_WRITE: {
        log_info(logger, "Kernel nos envio - KERNEL_IO_STDOUT_WRITE");
        if (cfg->TIPO_INTERFAZ != STDOUT)
        {
            // log_error(logger, "KERNEL NOS ENVIO UNA OPERACION PARA UNA IO STDOUT");
            mem_clean_exit();
        }
        t_paquete *paquete_send = resolver_KERNEL_IO_STDOUT_WRITE(paquete_reciv);
        send_paquete(fd_kernel, paquete_send);
        log_info(logger, "Se envia a Kernel - IO_KERNEL_STDOUT_WRITE");
        break;
    }
    default:
        // log_error(logger, "Recibimos de kernel un op code erroneo: %i, seguramente es culpa de juano",
        //           paquete_reciv->codigo_operacion);
        mem_clean_exit();
        break;
    }
}
