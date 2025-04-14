#ifndef CONTEXTO_EJECUCION_HANDLERS_H_
#define CONTEXTO_EJECUCION_HANDLERS_H_

#include <TAD/TAD.h>
#include <commons/log.h>
#include <globals/globals.h>
#include <pthread.h>

void init_mutex_contexto_ejecucion();
void destroy_mutex_contexto_ejecucion();

// Getters

uint32_t get_pid();
uint32_t get_pc();
uint8_t get_ax();
uint8_t get_bx();
uint8_t get_cx();
uint8_t get_dx();
uint32_t get_eax();
uint32_t get_ebx();
uint32_t get_ecx();
uint32_t get_edx();
uint32_t get_si();
uint32_t get_di();
t_contexto_ejecucion get_all();

// Setters

void set_pid(uint32_t new_pid);
void set_pc(uint32_t new_pc);
void set_ax(uint8_t new_ax);
void set_bx(uint8_t new_bx);
void set_cx(uint8_t new_cx);
void set_dx(uint8_t new_dx);
void set_eax(uint32_t new_eax);
void set_ebx(uint32_t new_ebx);
void set_ecx(uint32_t new_ecx);
void set_edx(uint32_t new_edx);
void set_si(uint32_t new_si);
void set_di(uint32_t new_di);
void set_all(t_contexto_ejecucion contexto);

void clear_contexto_ejecucion();
void print_contexto_ejec();

#endif