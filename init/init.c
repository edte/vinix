#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/sprintf.h"
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
    // disp_str_raw(0, 0, "enter kernel main");
    sprintf("hello:%s", "word");
    // printf("hello: %s", "world");

    // int i = 1 / 0;

    for (;;) {
    }
}
