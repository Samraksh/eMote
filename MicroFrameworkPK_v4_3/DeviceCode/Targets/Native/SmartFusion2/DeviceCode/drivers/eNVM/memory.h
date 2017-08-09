#ifndef _MEMORY_H_
#define _MEMORY_H_
#include <tinyhal.h>

void *malloc_flash(unsigned nbytes);
void free_flash(void* ap);
uint16_t* read_flash(void *ap);
void write_flash(void *ap,void *data, int nbytes);


#endif