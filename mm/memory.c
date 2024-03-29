#include "../include/memory.h"
#include "../include/mmu.h"
#include "../include/printk.h"
#include "../include/type.h"

void init_memory() {
    int i, j;
    unsigned long TotalMem = 0;
    struct Memory_E820_Formate *p = NULL;

    // kprintln("display mem:");
    p = (struct Memory_E820_Formate *)0x7e00;

    for (i = 0; i < 32; i++) {
        // kprintln("Address:%#010x,%08x,   Length:%#010x,%08x,   Type:%#010x", p->address2, p->address1, p->length2, p->length1, p->type);
        unsigned long tmp = 0;
        if (p->type == 1) {
            tmp = p->length2;
            TotalMem += p->length1;
            TotalMem += tmp << 32;
        }

        p++;
        if (p->type > 4)
            break;
    }

    kprintf("OS Can Used Total RAM:%x\n", TotalMem);
}
