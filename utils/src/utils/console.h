#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/// @brief Evita que la consola de los tests queden bloteadas con los logs
void consola_con_censura();

/// @brief Vuelve a permitir que los logs salgan por consola
void consola_con_libre_expresion();

#endif
