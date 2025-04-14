#ifndef LEER_LINEA_H_
#define LEER_LINEA_H_

#include <commons/string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool leer_linea(FILE *file, char *linea);

#define LINE_MAX 50

#endif