#include "gdt.h"
#include "lib.h"

void kernel_main(void) {
    clear();
    print(0, 0, "enter kernel main");

    init_gdt();

    // int i = 1 / 0;
    while (1)
        ;
}
