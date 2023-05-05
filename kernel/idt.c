#include "../include/idt.h"
#include "../include/asm.h"
#include "../include/gdt.h"
#include "../include/irq.h"
#include "../include/type.h"

// 设置门描述符
void init_idt_entry(idt_entry_t *p_entry, uint16_t selector, uint64_t offset, uint8_t ist, uint8_t type_attr) {
    p_entry->offset_low = (u16)offset;
    p_entry->offset_middle = (u16)(offset >> 16);
    p_entry->offset_high = (u32)(offset >> 32);
    p_entry->selector = selector;
    p_entry->ist = ist;
    p_entry->type_attr = type_attr;
    p_entry->reserved = 0;
}

void init_idt(void) {
    // 初始化所有中断
    init_irq();

    // 设置指针
    idt_pointer.base = (u64)idt_table;
    idt_pointer.limit = sizeof(idt_table) - 1;

    // 加载
    lidt();

    // 初始化pic 控制器
    init_pic();
}
