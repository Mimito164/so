#ifndef RESIZE_H_
#define RESIZE_H_

#include <estructuras_administrativas/bitmap_frames_libres.h>
#include <estructuras_administrativas/tabla_paginas.h>
#include <stdbool.h>
#include <stdint.h>

bool resize(uint32_t pid, uint32_t tamanio);

#endif