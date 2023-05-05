#include "../include/gdt.h"
#include "../include/lib.h"

void gdt_entry_set(int selector, u32 base, u32 limit, u16 attr) {
    gdt_entry_t *desc = gdt_table + selector / sizeof(gdt_entry_t);

    // 如果界限比较长，将长度单位换成4KB
    if (limit > 0xfffff) {
        attr |= 0x8000;
        limit /= 0x1000;
    }

    desc->limit_low = limit & 0xffff;
    desc->base_low = base & 0xffff;
    desc->base_mid = (base >> 16) & 0xff;
    desc->attr = attr | (((limit >> 16) & 0xf) << 8);
    desc->base_high = (base >> 24) & 0xff;
}

void init_gdt(void) {
    // 全部清空
    for (int i = 0; i < GDT_TABLE_SIZE; i++) {
        gdt_entry_set(i * sizeof(gdt_entry_t), 0, 0, 0);
    }

    // 内核代码段
    gdt_entry_set(SELECTOR_KERNEL_CODE, 0x00000000, 0xFFFFFFFF,
                  SEG_P_PRESENT | SEG_DPL0 | SEG_S_NORMAL | SEG_TYPE_CODE | SEG_TYPE_RW | SEG_L_64Bit);

    // 内核数据段
    gdt_entry_set(SELECTOR_KERNEL_DATA, 0x00000000, 0xFFFFFFFF,
                  SEG_P_PRESENT | SEG_DPL0 | SEG_S_NORMAL | SEG_TYPE_DATA | SEG_TYPE_RW | SEG_L_64Bit);

    // 内核栈段
    gdt_entry_set(SELECTOR_KERNEL_STACK, 0x00000000, 0xFFFFFFFF,
                  SEG_P_PRESENT | SEG_DPL0 | SEG_S_NORMAL | SEG_TYPE_DATA | SEG_TYPE_RW | SEG_L_64Bit);

    // 内核显存段
    gdt_entry_set(SELECTOR_VIDEO, 0x00000000, 0xFFFFFFFF,
                  SEG_P_PRESENT | SEG_DPL0 | SEG_S_NORMAL | SEG_TYPE_DATA | SEG_TYPE_RW | SEG_L_64Bit);

    // 设置 gdt 指针
    gdt_pointer.base = (u64)gdt_table;
    gdt_pointer.limit = sizeof(gdt_table) - 1;

    // 加载gdt
    lgdt();
}
