#include "send.h"

void send_Kernel_memoria_creacion(uint32_t pid, char *pseudocodigo_file_path)
{
    s_kernel_memoria_creacion_proceso s_k_m_creacion_pros = create_k_m_creacion_proceso(pid, pseudocodigo_file_path);
    t_paquete *paquete = serializar_k_m_creacion_proceso(s_k_m_creacion_pros);
    send_paquete(get_fd_memoria(), paquete);
}

void send_Kernel_memoria_finalizacion(uint32_t pid)
{
    log_trace(get_logger(), "Enviamos a memoria KERNEL_MEMORIA_FINALIZAR_PROCESO del proceso %i", pid);
    send_paquete(get_fd_memoria(), finalizar_proceso_memoria(pid));
}