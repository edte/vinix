#include "gdt.h"
#include "idt.h"
#include "lib.h"
#include "type.h"

extern void kernel_init();

void kernel_main();

void kernel_init(void) {
    clear();
    init_gdt();
    init_idt();

    goto entry;

entry:

    return;
}

void kernel_main(void) {
    print(0, 0, "enter kernel main");

    // int i = 1 / 0;

    for (;;) {
    }
}
