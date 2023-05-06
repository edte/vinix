#ifndef _ORANGES_IRQ_H_
#define _ORANGES_IRQ_H_

#include "type.h"

// 中断栈帧结构体类型
typedef struct {
    uint64_t rax;           /* 返回值 */
    uint64_t rbx;           /* 老寄存器的数值 */
    uint64_t rcx;           /* 函数调用前调用者需要保持的寄存器 （caller saved） */
    uint64_t rdx;           /* 函数调用传递参数，返回值被覆写（caller saved） */
    uint64_t rbp;           /* 基址指针，指向栈底 */
    uint64_t rsp;           /* 栈顶指针 */
    uint64_t rsi;           /* 函数调用前调用者需要保持的寄存器 （caller saved）*/
    uint64_t rdi;           /* 函数调用传递参数，返回值被覆写（caller saved） */
    uint64_t r8;            /* 函数调用前调用者需要保持的寄存器 （caller saved）*/
    uint64_t r9;            /* 函数调用传递参数，返回值被覆写（caller saved） */
    uint64_t r10;           /* 函数调用前调用者需要保持的寄存器 （caller saved）*/
    uint64_t r11;           /* 函数调用前调用者需要保持的寄存器 （caller saved）*/
    uint64_t r12;           /* 函数调用前被调用者需要保持的寄存器 （callee saved）*/
    uint64_t r13;           /* 函数调用前被调用者需要保持的寄存器 （callee saved）*/
    uint64_t r14;           /* 函数调用前被调用者需要保持的寄存器 （callee saved）*/
    uint64_t r15;           /* 函数调用前被调用者需要保持的寄存器 （callee saved）*/
    uint64_t rip;           /* 下一条指令地址 */
    uint64_t cs;            /* 代码段选择子 */
    uint64_t rflags;        /* 标志寄存器 */
    uint64_t rsp_at_signal; /* 发生中断时的栈顶指针 */
    uint64_t ss;            /* 堆栈段选择子 */
} __attribute__((packed)) irq_frame_t;

// 中断函数
typedef void (*irq_handler_t)(irq_frame_t *frame);

// 初始化所有中断处理函数
void init_irq();

// 设置一个中断处理函数
void irq_install(int irq_num, irq_handler_t handler);

/* 中断向量 */
#define IRQ0_DE 0
#define IRQ1_DB 1
#define IRQ2_NMI 2
#define IRQ3_BP 3
#define IRQ4_OF 4
#define IRQ5_BR 5
#define IRQ6_UD 6
#define IRQ7_NM 7
#define IRQ8_DF 8
#define IRQ10_TS 10
#define IRQ11_NP 11
#define IRQ12_SS 12
#define IRQ13_GP 13
#define IRQ14_PF 14
#define IRQ16_MF 16
#define IRQ17_AC 17
#define IRQ18_MC 18
#define IRQ19_XM 19
#define IRQ20_VE 20

/* 异常处理函数 */
void exception_handler_unknown(irq_frame_t *frame);
void exception_handler_divider(irq_frame_t *frame);
void exception_handler_Debug(irq_frame_t *frame);
void exception_handler_NMI(irq_frame_t *frame);
void exception_handler_breakpoint(irq_frame_t *frame);
void exception_handler_overflow(irq_frame_t *frame);
void exception_handler_bound_range(irq_frame_t *frame);
void exception_handler_invalid_opcode(irq_frame_t *frame);
void exception_handler_device_unavailable(irq_frame_t *frame);
void exception_handler_double_fault(irq_frame_t *frame);
void exception_handler_invalid_tss(irq_frame_t *frame);
void exception_handler_segment_not_present(irq_frame_t *frame);
void exception_handler_stack_segment_fault(irq_frame_t *frame);
void exception_handler_general_protection(irq_frame_t *frame);
void exception_handler_page_fault(irq_frame_t *frame);
void exception_handler_fpu_error(irq_frame_t *frame);
void exception_handler_alignment_check(irq_frame_t *frame);
void exception_handler_machine_check(irq_frame_t *frame);
void exception_handler_smd_exception(irq_frame_t *frame);
void exception_handler_virtual_exception(irq_frame_t *frame);

void irq_enable(int irq_num);
void irq_disable(int irq_num);
void irq_disable_global(void);
void irq_enable_global(void);

void init_pic(void);

// PIC控制器相关的寄存器及位配置
#define PIC0_ICW1 0x20
#define PIC0_ICW2 0x21
#define PIC0_ICW3 0x21
#define PIC0_ICW4 0x21
#define PIC0_OCW2 0x20
#define PIC0_IMR 0x21

#define PIC1_ICW1 0xa0
#define PIC1_ICW2 0xa1
#define PIC1_ICW3 0xa1
#define PIC1_ICW4 0xa1
#define PIC1_OCW2 0xa0
#define PIC1_IMR 0xa1

#define PIC_ICW1_ICW4 (1 << 0)     // 1 - 需要初始化ICW4
#define PIC_ICW1_ALWAYS_1 (1 << 4) // 总为1的位
#define PIC_ICW4_8086 (1 << 0)     // 8086工作模式

#define IRQ_PIC_START 0x20 // PIC中断起始号

#endif /* _ORANGES_IRQ_H_ */
