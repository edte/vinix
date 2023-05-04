#include "idt.h"
#include "gdt.h"
#include "lib.h"
#include "type.h"

// 中断处理函数，在此仅输出一句话作为示例
void handle_timer_interrupt(void) { print(3, 0, "Timer interrupt occurred\n"); }

// 设置门描述符
void init_idt_entry(idt_entry_t *p_entry, uint64_t offset, uint16_t selector, uint8_t ist, uint8_t type_attr) {
    p_entry->offset_low = (u16)offset;
    p_entry->offset_middle = (u16)(offset >> 16);
    p_entry->offset_high = (u32)(offset >> 32);
    p_entry->selector = selector;
    p_entry->ist = ist;
    p_entry->type_attr = type_attr;
    p_entry->reserved = 0;
}

void init_idt(void) {
    print(1, 0, "enter init idt");

    for (u32 i = 0; i < IDT_TABLE_SIZE; i++) {
        init_idt_entry(idt_table + i, (uint64_t)handle_timer_interrupt, 0x08, 0, 0x8E);
    }

    idt_pointer.base = (u64)idt_table;
    idt_pointer.limit = sizeof(idt_table) - 1;

    lidt();

    int i = 2 / 0;

    print(2, 0, "init idt succ");
}
