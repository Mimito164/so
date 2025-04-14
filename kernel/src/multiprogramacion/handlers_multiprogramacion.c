#include "handlers_multiprogramacion.h"

void increment_multiprogramacion()
{
    int multi = get_multiprogramacion();
    set_multiprogramacion(++multi);
    admit();
}

void increment_n_multiprogramacion(int times)
{
    for (int i = 0; i < times; i++)
    {
        increment_multiprogramacion();
    }
}

void decrement_multiprogramacion()
{
    int multi = get_multiprogramacion();
    set_multiprogramacion(--multi);
}

void decrement_n_multiprogramacion(int times)
{
    for (int i = 0; i < times; i++)
    {
        decrement_multiprogramacion();
    }
}

void update_multiprogramacion(int new_multiprogramacion)
{
    int prev_multiprogramacion = get_multiprogramacion();
    int diff = new_multiprogramacion - prev_multiprogramacion;
    if (diff == 0)
    {
        return;
    }
    if (diff > 0) // incrementar multiprog
    {
        increment_n_multiprogramacion(diff);
    }
    else
    {
        decrement_n_multiprogramacion(abs(diff));
    }
}