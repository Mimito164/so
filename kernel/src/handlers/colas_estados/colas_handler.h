#ifndef COLAS_HANDLER_H_
#define COLAS_HANDLER_H_

#include <assert.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <handlers/pcb_handler/pcb_handler.h>
#include <pthread.h>
#include <semaphore.h>

void initialize_new_list();
void destroy_new_list();
void initialize_ready_list();
void destroy_ready_list();
void initialize_blocked_list();
void destroy_blocked_list();
void initialize_exit_list();
void destroy_exit_list();
void initialize_executing();
void destroy_executing();
void set_executing(t_pcb *executing_pcb);
t_pcb *get_executing();
t_pcb *get_next_pcb(char *string_list);
void add_pcb(char *string_list, t_pcb *pcb);
t_pcb *find_pcb_by_pid(char *string_list, uint32_t pid);
void initialize_lists();
void destroy_lists();
int len_dict();
int len_list(char *string_list);
void print_all_lists();

extern pthread_mutex_t mutex_exec;

#endif
