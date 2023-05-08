#include "../include/asm.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/irq.h"
#include "../include/kprint.h"
#include "../include/lib.h"
#include "../include/memory.h"
#include "../include/printk.h"
#include "../include/time.h"
#include "../include/type.h"

void kernel_main();

void kernel_init(void) {
    init_gdt();
    init_idt();
    init_screen();
    init_memory();
    init_time();
    irq_enable_global();
    // irq_disable(0x20);
    // irq_disable(0x21);

    kernel_main();
}

void kernel_main(void) {
    kprintln("enter kernel main");
    // kprintf("%d\n", 10);

    // int i = 2 / 0;
    // for (int j = 0; j < 100; j++) {
    //     kprintf("%d\n", sys_tick);
    // }

    while (1)
        ;
}
