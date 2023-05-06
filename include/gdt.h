#ifndef _ORANGES_GDT_H_
#define _ORANGES_GDT_H_

#include "type.h"

#define reassembly(high, high_shift, mid, mid_shift, low) (((high) << (high_shift)) + ((mid) << (mid_shift)) + (low))
#define GDT_TABLE_SIZE 128 // GDT表项数量

// GDT 段描述符结构体
typedef struct {
    u16 limit_low; // gdt 限长低 16 位
    u16 base_low;  // gdt 起始地址低 16 位
    u8 base_mid;   // gdt 起始地址中间 8 位
    u16 attr;
    u8 base_high; // gdt 起始地址高 8 位
} __attribute__((packed)) gdt_entry_t;

// 段描述符属性
#define SEG_G_Granularity (1 << 15)                // 设置段界限的单位，1-4KB，0-字节
#define SEG_D_Default (1 << 14)                    // 控制是否是32位、16位的代码或数据段
#define SEG_L_64Bit (1 << 13)                      // 控制是不是 64 位
#define SEG_ALV_Available_To_Sys_Program (1 << 12) // todo:
#define SEG_P_PRESENT (1 << 7)                     // 段是否存在
#define SEG_DPL0 (0 << 5)                          // 特权级0，最高特权级
#define SEG_DPL3 (3 << 5)                          // 特权级3，最低权限
#define SEG_S_SYSTEM (0 << 4)                      // 是否是系统段，如调用门或者中断
#define SEG_S_NORMAL (1 << 4)                      // 普通的代码段或数据段
#define SEG_TYPE_CODE (1 << 3)                     // 指定其为代码段
#define SEG_TYPE_DATA (0 << 3)                     // 数据段
#define SEG_TYPE_RW (1 << 1)                       // 是否可写可读，不设置为只读

/* RPL */
#define RPL_KRNL SA_RPL0
#define RPL_TASK SA_RPL1
#define RPL_USER SA_RPL3

// 选择子
#define SELECTOR_KERNEL_CODE (1 * 8) // 内核代码段描述符
#define SELECTOR_KERNEL_DATA (2 * 8) // 内核数据段描述符
#define SELECTOR_USER_CODE (3 * 8)
#define SELECTOR_USER_DATA (4 * 8)
#define SELECTOR_TSS (5 * 8)
#define SELECTOR_VIDEO (6 * 8)
#define SELECTOR_KERNEL_STACK (7 * 8)

/* 选择子类型值说明 */
/* 其中, SA_ : Selector Attribute */
#define SA_RPL_MASK 0xFFFC
#define SA_RPL0 0
#define SA_RPL1 1
#define SA_RPL2 2
#define SA_RPL3 3
#define SA_TI_MASK 0xFFFB
#define SA_TIG 0
#define SA_TIL 4

// GDT 指针结构体
typedef struct {
    u16 limit; // gdt 限长
    u64 base;  // gdt 起始地址
} __attribute__((packed)) gdt_ptr_t;

// 定义 GDT 描述符数组
static gdt_entry_t gdt_table[GDT_TABLE_SIZE];

// 定义 GDT 指针
static gdt_ptr_t gdt_pointer;

// 初始化 gdt
void init_gdt(void);

#endif /* _ORANGES_GDT_H_ */
