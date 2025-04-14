#include "leer_linea.h"

bool leer_linea(FILE *file, char *linea)
{
    if (file == NULL)
    {
        return false;
    }

    if (fgets(linea, LINE_MAX, file) == NULL)
    {
        return false; // si termino el archivo
    }

    // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.strcpy)
    strcpy(linea, string_replace(linea, "\n", ""));

    return true;
}