#ifndef _ORANGES_IRQ_H_
#define _ORANGES_IRQ_H_

#include "type.h"

// 中断栈帧结构体类型
typedef struct {
    uint64_t rax;    /* 返回值 */
    uint64_t rbx;    /* 老寄存器的数值 */
    uint64_t rcx;    /* 函数调用前调用者需要保持的寄存器 （caller saved） */
    uint64_t rdx;    /* 函数调用传递参数，返回值被覆写（caller saved） */
    uint64_t rbp;    /* 基址指针，指向栈底 */
    uint64_t rsp;    /* 栈顶指针 */
    uint64_t rsi;    /* 函数调用前调用者需要保持的寄存器 （caller saved）*/
    uint64_t rdi;    /* 函数调用传递参数，返回值被覆写（caller saved） */
    uint64_t r8;     /* 函数调用前调用者需要保持的寄存器 （caller saved）*/
    uint64_t r9;     /* 函数调用传递参数，返回值被覆写（caller saved） */
    uint64_t r10;    /* 函数调用前调用者需要保持的寄存器 （caller saved）*/
    uint64_t r11;    /* 函数调用前调用者需要保持的寄存器 （caller saved）*/
    uint64_t r12;    /* 函数调用前被调用者需要保持的寄存器 （callee saved）*/
    uint64_t r13;    /* 函数调用前被调用者需要保持的寄存器 （callee saved）*/
    uint64_t r14;    /* 函数调用前被调用者需要保持的寄存器 （callee saved）*/
    uint64_t r15;    /* 函数调用前被调用者需要保持的寄存器 （callee saved）*/
    uint64_t rip;    /* 下一条指令地址 */
    uint64_t cs;     /* 代码段选择子 */
    uint64_t rflags; /* 标志寄存器 */
} __attribute__((packed)) irq_frame_t;

// 中断函数
typedef void (*irq_handler_t)(void);

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
void exception_handler_unknown(void);
void exception_handler_divider(void);
void exception_handler_Debug(void);
void exception_handler_NMI(void);
void exception_handler_breakpoint(void);
void exception_handler_overflow(void);
void exception_handler_bound_range(void);
void exception_handler_invalid_opcode(void);
void exception_handler_device_unavailable(void);
void exception_handler_double_fault(void);
void exception_handler_invalid_tss(void);
void exception_handler_segment_not_present(void);
void exception_handler_stack_segment_fault(void);
void exception_handler_general_protection(void);
void exception_handler_page_fault(void);
void exception_handler_fpu_error(void);
void exception_handler_alignment_check(void);
void exception_handler_machine_check(void);
void exception_handler_smd_exception(void);
void exception_handler_virtual_exception(void);
void exception_handler_timer(void);
void exception_handler_keyboard(void);

void init_8259A();
void irq_enable(int irq_num);
void irq_disable(int irq_num);
void irq_disable_global(void);
void irq_enable_global(void);

// PIC控制器相关的寄存器及位配置
#define PIC_M_CTRL 0x20 // 主片
#define PIC_M_DATA 0x21
#define PIC_S_CTRL 0xA0 // 从片
#define PIC_S_DATA 0xA1

#define IRQ_PIC_M_TIMER 0x20
#define IRQ_PIC_M_KERBOARD 0x21

#endif /* _ORANGES_IRQ_H_ */
