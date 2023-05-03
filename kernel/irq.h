#ifndef _ORANGES_IRQ_H_
#define _ORANGES_IRQ_H_

#include "type.h"

#define DA_386IGate 0x8E /* 386 中断门类型值			*/

// i8259a
#define INT_M_CTL 0x20     // I/O port for interrupt controller       <Master>
#define INT_M_CTLMASK 0x21 // setting bits in this port disables ints <Master>
#define INT_S_CTL 0xA0     // I/O port for second interrupt controller<Slave>
#define INT_S_CTLMASK 0xA1 // setting bits in this port disables ints <Slave>/

/* 中断向量 */
#define INT_VECTOR_DIVIDE 0x0
#define INT_VECTOR_DEBUG 0x1
#define INT_VECTOR_NMI 0x2
#define INT_VECTOR_BREAKPOINT 0x3
#define INT_VECTOR_OVERFLOW 0x4
#define INT_VECTOR_BOUNDS 0x5
#define INT_VECTOR_INVAL_OP 0x6
#define INT_VECTOR_COPROC_NOT 0x7
#define INT_VECTOR_DOUBLE_FAULT 0x8
#define INT_VECTOR_COPROC_SEG 0x9
#define INT_VECTOR_INVAL_TSS 0xA
#define INT_VECTOR_SEG_NOT 0xB
#define INT_VECTOR_STACK_FAULT 0xC
#define INT_VECTOR_PROTECTION 0xD
#define INT_VECTOR_PAGE_FAULT 0xE
#define INT_VECTOR_COPROC_ERR 0x10
#define INT_VECTOR_IRQ0 0x20
#define INT_VECTOR_IRQ8 0x28

// 中断函数
typedef void (*int_handler)();

/* 中断处理函数 */
void divide_error();
void single_step_exception();
void nmi();
void breakpoint_exception();
void overflow();
void bounds_check();
void inval_opcode();
void copr_not_available();
void double_fault();
void copr_seg_overrun();
void inval_tss();
void segment_not_present();
void stack_exception();
void general_protection();
void page_fault();
void copr_error();

struct idt_entry {
    u16 offset_lo;        // 中断处理程序在目标代码段内的偏移量（低 16 位）
    u16 segment_selector; // 调用中断处理程序的代码段描述符选择子
    u8 ist;               // 中断堆栈表索引（IST） （如果未使用，则为0）
    u8 type_attr;         // 标志/类型
    u16 offset_mid;       // 中断处理程序在目标代码段内的偏移量（中间16位）
    u32 offset_hi : 32,   // 中断处理程序在目标代码段内的偏移量（高 32 位），全为 0。
        reserved : 24;
} __attribute__((packed));

struct idt_descriptor {
    u16 limit;  //   IDT 表限制大小
    void *base; //   IDT 表基址指针
} __attribute__((packed));

// 定义 IDT 表和指针
extern struct idt_entry IDT_Table[];
extern struct idt_descriptor IDT_POINTER;

// 函数原型，汇编语言实现
void lidt(void *);

#endif /* _ORANGES_IRQ_H_ */
