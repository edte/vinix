#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/kprint.h"
#include "../include/type.h"

extern void kernel_init();

void kernel_init(void) {
    clear();
    init_gdt();
    init_idt();

    goto entry;

entry:

    return;
}

void kernel_main(void) {

    int i = 1 / 0;
    // disp_str_raw(5, 0, "enter kernel main");

    // int *addr = (int *)0xb8000;
    // *((char *)addr + 5) = '9';

    kprintf("hello:%s", "word");
    // sprintf("hello wolrd");
    // sprintf("hello:%x", 01234);
    // printf("hello: %s", "world");

    for (;;) {
    }
}
