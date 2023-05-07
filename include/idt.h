#ifndef _ORANGES_IDT_H_
#define _ORANGES_IDT_H_

#include "type.h"

// offset_low：中断处理程序的低 16 位偏移地址。
// selector：中断处理程序所在代码段的段选择子。
// ist：可选的中断栈表索引，可以用于指定硬件支持的重叠中断的栈表索引 (0 表示使用主堆栈)。
// type_attr：中断类型和属性，包括以下各位的值:
// D: 1 表示该门描述符为中断适配器 (也称为陷阱门)，0 表示该门描述符为任务门。在 x86
// 上任务门已经被废弃了，它们仍然存在（暂时）只是为了向后兼容性。 DT: 1 表示使用了 32 位中断门形式，在 x86_64
// 中不应设置为 1. DPL: 描述符特权级（Descriptor Privilege Level），用于控制用户态与内核态的访问权限。
// P：表示该门描述符是否有效。
// 剩余位数保留，始终设置为 0。
// offset_middle：中断处理程序的中间 16 位偏移地址。
// offset_high：中断处理程序的高 32 位偏移地址。
// reserved：保留字段，始终设置为 0。
typedef struct {
    u16 offset_low;    // 中断处理程序的低16位偏移地址
    u16 selector;      // 中断处理程序所在代码段的段选择子
    u8 ist;            // 中断栈表
    u8 type_attr;      // 中断类型和属性
    u16 offset_middle; // 中断处理程序的中间16位偏移地址
    u32 offset_high;   // 中断处理程序的高32位偏移地址
    u32 reserved;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    u16 limit; //   IDT 表限制大小
    u64 base;  //   IDT 表基址指针
} __attribute__((packed)) idt_ptr_t;

// 中断或异常类型（Type）宏定义
#define GATE_TYPE_INTERRUPT 0xe // 中断门
#define GATE_TYPE_TRAP 0xf      // 陷阱门
#define GATE_TYPE_TASK 0x5      // 任务门

// 特权级（DPL）宏定义
#define GATE_DPL0 (0 << 5) // 最高特权级
#define GATE_DPL1 (1 << 5)
#define GATE_DPL2 (2 << 5)
#define GATE_DPL3 (3 << 5) // 最低权限

// 存在位（P）
#define GATE_P_PRESENT (1 << 7) // 存在

#define IDT_TABLE_SIZE 256 // GDT表项数量

// 定义 IDT 表和指针
static idt_entry_t idt_table[IDT_TABLE_SIZE];
// 定义 IDT 指针
static idt_ptr_t idt_pointer;

// 初始化 idt
void init_idt();

void init_idt_entry(idt_entry_t *p_entry, uint16_t selector, uint64_t offset, uint8_t ist, uint8_t type_attr);

#endif /* _ORANGES_IDT_H_ */
