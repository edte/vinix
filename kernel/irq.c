#include "../include/irq.h"
#include "../include/asm.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/kprint.h"
#include "../include/time.h"

static void dump_core_regs(irq_frame_t *frame) {
    kprintln("rax:0x%x\n"
             "rbx:0x%x\n"
             "rcx:0x%x\n"
             "rdx:0x%x\n"
             "rsi:0x%x\n"
             "rdi:0x%x\n"
             "r8:0x%x\n"
             "r9:0x%x\n",
             "r10:0x%x\n", "r11:0x%x\n", "r12:0x%x\n", "r13:0x%x\n", "r14:0x%x\n", "r15:0x%x\n", "rip:0x%x\n", "cs:0x%x\n", "rflags:0x%x\n",
             "rsp:0x%x\n", "ss:0x%x\n", frame->rax, frame->rbx, frame->rcx, frame->rdx, frame->rsi, frame->rdi, frame->r8, frame->r9, frame->r10,
             frame->r11, frame->r12, frame->r13, frame->r14, frame->r15, frame->rip, frame->cs, frame->rflags, frame->rsp, frame->ss);
}

static void do_default_handler(irq_frame_t *frame, const char *message) {
    disp_pos = 0;
    kprintln("-------------------------------------------------------");
    kprintln("IRQ/Exception happend: %s", message);
    dump_core_regs(frame);

    // todo: 留等以后补充打印任务栈的内容

    kprintln("-------------------------------------------------------");
    for (;;) {
        hlt();
    }
}

void exception_handler_unknown(irq_frame_t *frame) { do_default_handler(frame, "Unknown exception."); };
void exception_handler_divider(irq_frame_t *frame) { do_default_handler(frame, "Divider Error."); };
void exception_handler_Debug(irq_frame_t *frame) { do_default_handler(frame, "Debug Exception"); };
void exception_handler_NMI(irq_frame_t *frame) { do_default_handler(frame, "NMI Interrupt."); };
void exception_handler_breakpoint(irq_frame_t *frame) { do_default_handler(frame, "Breakpoint."); };
void exception_handler_overflow(irq_frame_t *frame) { do_default_handler(frame, "Overflow."); };
void exception_handler_bound_range(irq_frame_t *frame) { do_default_handler(frame, "BOUND Range Exceeded."); };
void exception_handler_invalid_opcode(irq_frame_t *frame) { do_default_handler(frame, "Invalid Opcode."); };
void exception_handler_device_unavailable(irq_frame_t *frame) { do_default_handler(frame, "Device Not Available."); };
void exception_handler_double_fault(irq_frame_t *frame) { do_default_handler(frame, "Double Fault."); };
void exception_handler_invalid_tss(irq_frame_t *frame) { do_default_handler(frame, "Invalid TSS"); };
void exception_handler_segment_not_present(irq_frame_t *frame) { do_default_handler(frame, "Segment Not Present."); };
void exception_handler_stack_segment_fault(irq_frame_t *frame) { do_default_handler(frame, "Stack-Segment Fault."); };
void exception_handler_general_protection(irq_frame_t *frame) { do_default_handler(frame, "General Protection."); };
void exception_handler_page_fault(irq_frame_t *frame) { do_default_handler(frame, "Page Fault."); };
void exception_handler_fpu_error(irq_frame_t *frame) { do_default_handler(frame, "X87 FPU Floating Point Error."); };
void exception_handler_alignment_check(irq_frame_t *frame) { do_default_handler(frame, "Alignment Check."); };
void exception_handler_machine_check(irq_frame_t *frame) { do_default_handler(frame, "Machine Check."); };
void exception_handler_smd_exception(irq_frame_t *frame) { do_default_handler(frame, "SIMD Floating Point Exception."); };
void exception_handler_virtual_exception(irq_frame_t *frame) { do_default_handler(frame, "Virtualization Exception."); };

void exception_handler_timer(irq_frame_t *frame) {
    sys_tick++;

    kprintln("%d", sys_tick);
    iretq();
}

void exception_handler_keyboard(irq_frame_t *frame) {
    outb(0x20, 0x61);
    uint16_t scancode = inb(0x60);
    kprintln("");
    kprintln("%d", scancode);
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
    iretq();
}

/*======================================================================*
                            init_8259A
 *======================================================================*/
void init_8259A() {
    // 初始化主片
    outb(PIC_M_CTRL, 0x11); // ICW1: 0001 0001 ,边沿触发，级联 8259，需要ICW4
    outb(PIC_M_DATA, 0x20); // ICW2: 0010 0000 ,起始中断向量号为 0x20(0x20-0x27)
    outb(PIC_M_DATA, 0x04); // ICW3: 0000 0100 ,IR2 接从片
    outb(PIC_M_DATA, 0x01); // ICW4: 0000 0001 ,8086 模式，正常EOI

    // 初始化从片
    outb(PIC_S_CTRL, 0x11); // ICW1: 0001 0001 ,边沿触发，级联 8259，需要ICW4
    outb(PIC_S_DATA, 0x28); // ICW2: 0010 1000 ,起始中断向量号为 0x28(0x28-0x2f)
    outb(PIC_S_DATA, 0x02); // ICW3: 0000 0010 ,设置连接到主片的 IR2 引脚
    outb(PIC_S_DATA, 0x01); // ICW4: 0000 0001 ,8086 模式，正常EOI

    /* IRQ2用于级联从片,必须打开,否则无法响应从片上的中断
    主片上打开的中断有IRQ0的时钟,IRQ1的键盘和级联从片的IRQ2,其它全部关闭 */
    outb(PIC_M_DATA, 0xf8);

    /* 打开从片上的IRQ14,此引脚接收硬盘控制器的中断 */
    outb(PIC_S_DATA, 0xff);
}

void irq_disable_global(void) { cli(); }

void irq_enable_global(void) { sti(); }
