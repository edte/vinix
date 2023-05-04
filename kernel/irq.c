#include "irq.h"
// #include "gate.h"
#include "lib.h"

/*======================================================================*
                            init_8259A
 *======================================================================*/
// void init_8259A() {
//     out_byte(INT_M_CTL, 0x11);                // Master 8259, ICW1.
//     out_byte(INT_S_CTL, 0x11);                // Slave  8259, ICW1.
//     out_byte(INT_M_CTLMASK, INT_VECTOR_IRQ0); // Master 8259, ICW2. 设置 '主8259' 的中断入口地址为 0x20.
//     out_byte(INT_S_CTLMASK, INT_VECTOR_IRQ8); // Slave  8259, ICW2. 设置 '从8259' 的中断入口地址为 0x28
//     out_byte(INT_M_CTLMASK, 0x4);             // Master 8259, ICW3. IR2 对应 '从8259'.
//     out_byte(INT_S_CTLMASK, 0x2);             // Slave  8259, ICW3. 对应 '主8259' 的 IR2.
//     out_byte(INT_M_CTLMASK, 0x1);             // Master 8259, ICW4.
//     out_byte(INT_S_CTLMASK, 0x1);             // Slave  8259, ICW4.

//     out_byte(INT_M_CTLMASK, 0xFF); // Master 8259, OCW1.
//     out_byte(INT_S_CTLMASK, 0xFF); // Slave  8259, OCW1.

//     int i;
//     // for (i = 0; i < NR_IRQ; i++) {
//     //     irq_table[i] = spurious_irq;
//     // }
// }

// /*======================================================================*
//                            spurious_irq
//  *======================================================================*/
// void spurious_irq(int irq) {
//     disp_str("spurious_irq: ");
//     disp_int(irq);
//     disp_str("\n");
// }

// /*======================================================================*
//                            put_irq_handler
//  *======================================================================*/
// void put_irq_handler(int irq, irq_handler handler) {
//     disable_irq(irq);
//     irq_table[irq] = handler;
// }

// /*======================================================================*
//                              init_idt_desc
//  *----------------------------------------------------------------------*
//  初始化 386 中断门
//  *======================================================================*/
// void init_idt_desc(unsigned char vector, u8 desc_type, int_handler handler, unsigned char privilege) {
//     GATE *p_gate = &IDT_Table[vector];
//     u32 base = (u32)handler;
//     p_gate->offset_low = base & 0xFFFF;
//     p_gate->selector = SELECTOR_KERNEL_CS;
//     p_gate->dcount = 0;
//     p_gate->attr = desc_type | (privilege << 5);
//     p_gate->offset_high = (base >> 16) & 0xFFFF;
// }

// /*======================================================================*
//                             init_prot
//  *======================================================================*/
// void init_prot() {
//     // 全部初始化成中断门(没有陷阱门)
//     init_idt_desc(INT_VECTOR_DIVIDE, DA_386IGate, divide_error, PRIVILEGE_KRNL);

//     init_idt_desc(INT_VECTOR_DEBUG, DA_386IGate, single_step_exception, PRIVILEGE_KRNL);

//     init_idt_desc(INT_VECTOR_NMI, DA_386IGate, nmi, PRIVILEGE_KRNL);

//     init_idt_desc(INT_VECTOR_BREAKPOINT, DA_386IGate, breakpoint_exception, PRIVILEGE_USER);

//     init_idt_desc(INT_VECTOR_OVERFLOW, DA_386IGate, overflow, PRIVILEGE_USER);

//     init_idt_desc(INT_VECTOR_BOUNDS, DA_386IGate, bounds_check, PRIVILEGE_KRNL);

//     init_idt_desc(INT_VECTOR_INVAL_OP, DA_386IGate, inval_opcode, PRIVILEGE_KRNL);

//     init_idt_desc(INT_VECTOR_COPROC_NOT, DA_386IGate, copr_not_available, PRIVILEGE_KRNL);

//     init_idt_desc(INT_VECTOR_DOUBLE_FAULT, DA_386IGate, double_fault, PRIVILEGE_KRNL);

//     init_idt_desc(INT_VECTOR_COPROC_SEG, DA_386IGate, copr_seg_overrun, PRIVILEGE_KRNL);

//     init_idt_desc(INT_VECTOR_INVAL_TSS, DA_386IGate, inval_tss, PRIVILEGE_KRNL);

//     init_idt_desc(INT_VECTOR_SEG_NOT, DA_386IGate, segment_not_present, PRIVILEGE_KRNL);

//     init_idt_desc(INT_VECTOR_STACK_FAULT, DA_386IGate, stack_exception, PRIVILEGE_KRNL);

//     init_idt_desc(INT_VECTOR_PROTECTION, DA_386IGate, general_protection, PRIVILEGE_KRNL);

//     init_idt_desc(INT_VECTOR_PAGE_FAULT, DA_386IGate, page_fault, PRIVILEGE_KRNL);

//     init_idt_desc(INT_VECTOR_COPROC_ERR, DA_386IGate, copr_error, PRIVILEGE_KRNL);
// }

// /*======================================================================*
//                             exception_handler
//  *----------------------------------------------------------------------*
//  异常处理
//  *======================================================================*/
// void exception_handler(int vec_no, int err_code, int eip, int cs, int eflags) {
//     int *addr = (int *)0xb8000;
//     // int i;

//     *((char *)addr + 1024) = (char)0xf0;

//     int disp_pos = 0;

//     int i;
//     int text_color = 0x74; /* 灰底红字 */

//     char *err_msg[] = {"#DE Divide Error",
//                        "#DB RESERVED",
//                        "--  NMI Interrupt",
//                        "#BP Breakpoint",
//                        "#OF Overflow",
//                        "#BR BOUND Range Exceeded",
//                        "#UD Invalid Opcode (Undefined Opcode)",
//                        "#NM Device Not Available (No Math Coprocessor)",
//                        "#DF Double Fault",
//                        "    Coprocessor Segment Overrun (reserved)",
//                        "#TS Invalid TSS",
//                        "#NP Segment Not Present",
//                        "#SS Stack-Segment Fault",
//                        "#GP General Protection",
//                        "#PF Page Fault",
//                        "--  (Intel reserved. Do not use.)",
//                        "#MF x87 FPU Floating-Point Error (Math Fault)",
//                        "#AC Alignment Check",
//                        "#MC Machine Check",
//                        "#XF SIMD Floating-Point Exception"};

//     /* 通过打印空格的方式清空屏幕的前五行，并把 disp_pos 清零 */
//     disp_pos = 0;
//     for (i = 0; i < 80 * 5; i++) {
//         disp_str(" ");
//     }
//     disp_pos = 0;

//     disp_color_str("Exception! --> ", text_color);
//     disp_color_str(err_msg[vec_no], text_color);
//     disp_color_str("\n\n", text_color);
//     disp_color_str("EFLAGS:", text_color);
//     disp_int(eflags);
//     disp_color_str("CS:", text_color);
//     disp_int(cs);
//     disp_color_str("EIP:", text_color);
//     disp_int(eip);

//     if (err_code != 0xFFFFFFFF) {
//         disp_color_str("Error code:", text_color);
//         disp_int(err_code);
//     }
// }
