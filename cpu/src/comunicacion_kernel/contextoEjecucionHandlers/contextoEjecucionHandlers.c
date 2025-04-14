#include "contextoEjecucionHandlers.h"

static t_contexto_ejecucion contexto_ejecucion;

// Getters
uint32_t get_pid()
{
    uint32_t temp_pid;
    temp_pid = contexto_ejecucion.pid;
    return temp_pid;
}

uint32_t get_pc()
{
    uint32_t temp_pc;
    temp_pc = contexto_ejecucion.registros_cpu.PC;
    return temp_pc;
}

uint8_t get_ax()
{
    uint8_t temp_ax;
    temp_ax = contexto_ejecucion.registros_cpu.AX;
    return temp_ax;
}

uint8_t get_bx()
{
    uint8_t temp_bx;
    temp_bx = contexto_ejecucion.registros_cpu.BX;
    return temp_bx;
}

uint8_t get_cx()
{
    uint8_t temp_cx;
    temp_cx = contexto_ejecucion.registros_cpu.CX;
    return temp_cx;
}

uint8_t get_dx()
{
    uint8_t temp_dx;
    temp_dx = contexto_ejecucion.registros_cpu.DX;
    return temp_dx;
}

uint32_t get_eax()
{
    uint32_t temp_eax;
    temp_eax = contexto_ejecucion.registros_cpu.EAX;
    return temp_eax;
}

uint32_t get_ebx()
{
    uint32_t temp_ebx;
    temp_ebx = contexto_ejecucion.registros_cpu.EBX;
    return temp_ebx;
}

uint32_t get_ecx()
{
    uint32_t temp_ecx;
    temp_ecx = contexto_ejecucion.registros_cpu.ECX;
    return temp_ecx;
}

uint32_t get_edx()
{
    uint32_t temp_edx;
    temp_edx = contexto_ejecucion.registros_cpu.EDX;
    return temp_edx;
}

uint32_t get_si()
{
    uint32_t temp_si;
    temp_si = contexto_ejecucion.registros_cpu.SI;
    return temp_si;
}

uint32_t get_di()
{
    uint32_t temp_di;
    temp_di = contexto_ejecucion.registros_cpu.DI;
    return temp_di;
}

void print_contexto_ejec()
{
    uint32_t pid = get_pid();
    uint32_t pc = get_pc();
    uint8_t ax = get_ax();
    uint8_t bx = get_bx();
    uint8_t cx = get_cx();
    uint8_t dx = get_dx();
    uint32_t eax = get_eax();
    uint32_t ebx = get_ebx();
    uint32_t ecx = get_ecx();
    uint32_t edx = get_edx();
    uint32_t si = get_si();
    uint32_t di = get_di();

    log_debug(
        get_logger(),
        "Contexto ejec: \n pid %i, pc %i, ax %i, bx %i, cx %i, dx %i, eax %i, ebx %i, ecx %i, edx %i, si %i, di %i",
        pid, pc, ax, bx, cx, dx, eax, ebx, ecx, edx, si, di);
}

// Setters

void set_pid(uint32_t new_pid)
{
    contexto_ejecucion.pid = new_pid;
}

void set_pc(uint32_t new_pc)
{
    contexto_ejecucion.registros_cpu.PC = new_pc;
}

void set_ax(uint8_t new_ax)
{
    contexto_ejecucion.registros_cpu.AX = new_ax;
    log_debug(get_logger(), "Valor actual del registro AX: %d", contexto_ejecucion.registros_cpu.AX);
}

void set_bx(uint8_t new_bx)
{
    contexto_ejecucion.registros_cpu.BX = new_bx;
    log_debug(get_logger(), "Valor actual del registro BX: %d", contexto_ejecucion.registros_cpu.BX);
}

void set_cx(uint8_t new_cx)
{
    contexto_ejecucion.registros_cpu.CX = new_cx;
    log_debug(get_logger(), "Valor actual del registro CX: %d", contexto_ejecucion.registros_cpu.CX);
}

void set_dx(uint8_t new_dx)
{
    contexto_ejecucion.registros_cpu.DX = new_dx;
    log_debug(get_logger(), "Valor actual del registro DX: %d", contexto_ejecucion.registros_cpu.DX);
}

void set_eax(uint32_t new_eax)
{
    contexto_ejecucion.registros_cpu.EAX = new_eax;
    log_debug(get_logger(), "Valor actual del registro EAX: %lu", (unsigned long)contexto_ejecucion.registros_cpu.EAX);
}

void set_ebx(uint32_t new_ebx)
{
    contexto_ejecucion.registros_cpu.EBX = new_ebx;
    log_debug(get_logger(), "Valor actual del registro EBX: %lu", (unsigned long)contexto_ejecucion.registros_cpu.EBX);
}

void set_ecx(uint32_t new_ecx)
{
    contexto_ejecucion.registros_cpu.ECX = new_ecx;
    log_debug(get_logger(), "Valor actual del registro ECX: %lu", (unsigned long)contexto_ejecucion.registros_cpu.ECX);
}

void set_edx(uint32_t new_edx)
{
    contexto_ejecucion.registros_cpu.EDX = new_edx;
    log_debug(get_logger(), "Valor actual del registro EDX: %lu", (unsigned long)contexto_ejecucion.registros_cpu.EDX);
}

void set_si(uint32_t new_si)
{
    contexto_ejecucion.registros_cpu.SI = new_si;
}

void set_di(uint32_t new_di)
{
    contexto_ejecucion.registros_cpu.DI = new_di;
}

void clear_contexto_ejecucion()
{
    set_pid(0);
    set_pc(0);
    set_ax(0);
    set_bx(0);
    set_cx(0);
    set_dx(0);
    set_eax(0);
    set_ebx(0);
    set_ecx(0);
    set_edx(0);
    set_si(0);
    set_di(0);
}

t_contexto_ejecucion get_all()
{
    t_contexto_ejecucion ctx;
    ctx.pid = get_pid();
    ctx.registros_cpu.PC = get_pc();
    ctx.registros_cpu.AX = get_ax();
    ctx.registros_cpu.BX = get_bx();
    ctx.registros_cpu.CX = get_cx();
    ctx.registros_cpu.DX = get_dx();
    ctx.registros_cpu.EAX = get_eax();
    ctx.registros_cpu.EBX = get_ebx();
    ctx.registros_cpu.ECX = get_ecx();
    ctx.registros_cpu.EDX = get_edx();
    ctx.registros_cpu.SI = get_si();
    ctx.registros_cpu.DI = get_di();
    return ctx;
}

void set_all(t_contexto_ejecucion contexto)
{
    contexto_ejecucion = contexto;
}