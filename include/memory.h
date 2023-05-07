#ifndef MEMORY_H
#define MEMORY_H

#include "../include/bitmap.h"

struct Memory_E820_Formate {
    unsigned int address1;
    unsigned int address2;
    unsigned int length1;
    unsigned int length2;
    unsigned int type;
};

void init_memory();

#endif // MEMORY_H
