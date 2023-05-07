#include "../include/asm.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/irq.h"
#include "../include/kprint.h"
#include "../include/memory.h"
#include "../include/time.h"
#include "../include/type.h"

extern void kernel_init();

void kernel_init(void) {
    clear();
    init_gdt();
    init_idt();
    init_time();
    irq_enable_global();
    irq_disable(0x20);
    // irq_disable(0x21);

    goto entry;

entry:

    return;
}

void kernel_main(void) {
    // int i = 1 / 0;

    kprintln("enter kernel main");

    // int *addr = (int *)0xb8000;
    // *((char *)addr + 5) = '9';

    // sprintf("hello wolrd");
    // sprintf("hello:%x", 01234);
    // printf("hello: %s", "world");

    hlt();
}
