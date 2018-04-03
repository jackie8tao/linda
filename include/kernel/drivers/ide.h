#ifndef __LINDA_IDE_H
#define __LINDA_IDE_H

#include <types.h>

void ide_init();

int ide_read(uint_t);
int ide_write(uint_t, char*);

#endif
