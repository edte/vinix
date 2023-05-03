#ifndef _ORANGES_GATE_H_
#define _ORANGES_GATE_H_

#include "type.h"

/* 存储段描述符/系统段描述符 */
typedef struct s_descriptor /* 共 8 个字节 */
{
    u16 limit_low;       /* Limit */
    u16 base_low;        /* Base */
    u8 base_mid;         /* Base */
    u8 attr1;            /* P(1) DPL(2) DT(1) TYPE(4) */
    u8 limit_high_attr2; /* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
    u8 base_high;        /* Base */
} DESCRIPTOR;

/* 门描述符 */
typedef struct s_gate {
    u16 offset_low;  /* Offset Low */
    u16 selector;    /* Selector */
    u8 dcount;       /* 该字段只在调用门描述符中有效。如果在利用
                        调用门调用子程序时引起特权级的转换和堆栈
                        的改变，需要将外层堆栈中的参数复制到内层
                        堆栈。该双字计数字段就是用于说明这种情况
                        发生时，要复制的双字参数的数量。*/
    u8 attr;         /* P(1) DPL(2) DT(1) TYPE(4) */
    u16 offset_high; /* Offset High */
} GATE;

typedef struct s_tss {
    u32 backlink;
    u32 esp0; /* stack pointer to use during interrupt */
    u32 ss0;  /*   "   segment  "  "    "        "     */
    u32 esp1;
    u32 ss1;
    u32 esp2;
    u32 ss2;
    u32 cr3;
    u32 eip;
    u32 flags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u32 es;
    u32 cs;
    u32 ss;
    u32 ds;
    u32 fs;
    u32 gs;
    u32 ldt;
    u16 trap;
    u16 iobase; /* I/O位图基址大于或等于TSS段界限，就表示没有I/O许可位图 */
                /*u8	iomap[2];*/
} TSS;

/* GDT */
/* 描述符索引 */
#define INDEX_DUMMY 0   // ┓
#define INDEX_FLAT_C 1  // ┣ LOADER 里面已经确定了的.
#define INDEX_FLAT_RW 2 // ┃
#define INDEX_VIDEO 3   // ┛
#define INDEX_TSS 4
#define INDEX_LDT_FIRST 5

/* 选择子 */
#define SELECTOR_DUMMY 0          // ┓
#define SELECTOR_FLAT_C 0x08      // ┣ LOADER 里面已经确定了的.
#define SELECTOR_FLAT_RW 0x10     // ┃
#define SELECTOR_VIDEO (0x18 + 3) // ┛<-- RPL=3
#define SELECTOR_TSS 0x20         // TSS. 从外层跳到内存时 SS 和 ESP 的值从里面获得.
#define SELECTOR_LDT_FIRST 0x28

#define SELECTOR_KERNEL_CS SELECTOR_FLAT_C
#define SELECTOR_KERNEL_DS SELECTOR_FLAT_RW
#define SELECTOR_KERNEL_GS SELECTOR_VIDEO

// #define SELECTOR_KERNEL_CS 0x8
// #define SELECTOR_KERNEL_DS 0x10

/* 描述符类型值说明 */
#define DA_32 0x4000       /* 32 位段				*/
#define DA_LIMIT_4K 0x8000 /* 段界限粒度为 4K 字节			*/
#define DA_DPL0 0x00       /* DPL = 0				*/
#define DA_DPL1 0x20       /* DPL = 1				*/
#define DA_DPL2 0x40       /* DPL = 2				*/
#define DA_DPL3 0x60       /* DPL = 3				*/
/* 存储段描述符类型值说明 */
#define DA_DR 0x90   /* 存在的只读数据段类型值		*/
#define DA_DRW 0x92  /* 存在的可读写数据段属性值		*/
#define DA_DRWA 0x93 /* 存在的已访问可读写数据段类型值	*/
#define DA_C 0x98    /* 存在的只执行代码段属性值		*/
#define DA_CR 0x9A   /* 存在的可执行可读代码段属性值		*/
#define DA_CCO 0x9C  /* 存在的只执行一致代码段属性值		*/
#define DA_CCOR 0x9E /* 存在的可执行可读一致代码段属性值	*/
/* 系统段描述符类型值说明 */
#define DA_LDT 0x82      /* 局部描述符表段类型值			*/
#define DA_TaskGate 0x85 /* 任务门类型值				*/
#define DA_386TSS 0x89   /* 可用 386 任务状态段类型值		*/
#define DA_386CGate 0x8C /* 386 调用门类型值			*/
#define DA_386IGate 0x8E /* 386 中断门类型值			*/
#define DA_386TGate 0x8F /* 386 陷阱门类型值			*/

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

/* 权限 */
#define PRIVILEGE_KRNL 0
#define PRIVILEGE_TASK 1
#define PRIVILEGE_USER 3
/* RPL */
#define RPL_KRNL SA_RPL0
#define RPL_TASK SA_RPL1
#define RPL_USER SA_RPL3

// GDT 描述符结构体
typedef struct {
    u16 limit_low;  // gdt 限长低 16 位
    u16 base_low;   // gdt 起始地址低 16 位
    u8 base_middle; // gdt 起始地址中间 8 位
    u8 access;      // 访问标志
    u8 granularity; // 段界限高 4 位和其它标志位
    u8 base_high;   // gdt 起始地址高 8 位
} __attribute__((packed)) gdt_entry_t;

// GDT 指针结构体
typedef struct {
    u16 limit; // gdt 限长
    u64 base;  // gdt 起始地址
} __attribute__((packed)) gdt_ptr_t;

// 定义 GDT 描述符数组
extern gdt_entry_t GDT_Table[];

// 定义 GDT 指针
extern gdt_ptr_t GDT_POINTER;

void init_gdt(void);

#define GDT_TABLE_SIZE 256 // GDT表项数量

#endif /* _ORANGES_GATE_H_ */
