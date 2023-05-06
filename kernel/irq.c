#include "../include/irq.h"
#include "../include/asm.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/kprint.h"

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

void init_pic(void) {
    // 边缘触发，级联，需要配置icw4, 8086模式
    outb(PIC0_ICW1, PIC_ICW1_ALWAYS_1 | PIC_ICW1_ICW4);

    // 对应的中断号起始序号0x20
    outb(PIC0_ICW2, IRQ_PIC_START);

    // 主片IRQ2有从片
    outb(PIC0_ICW3, 1 << 2);

    // 普通全嵌套、非缓冲、非自动结束、8086模式
    outb(PIC0_ICW4, PIC_ICW4_8086);

    // 边缘触发，级联，需要配置icw4, 8086模式
    outb(PIC1_ICW1, PIC_ICW1_ICW4 | PIC_ICW1_ALWAYS_1);

    // 起始中断序号，要加上8
    outb(PIC1_ICW2, IRQ_PIC_START + 8);

    // 没有从片，连接到主片的IRQ2上
    outb(PIC1_ICW3, 2);

    // 普通全嵌套、非缓冲、非自动结束、8086模式
    outb(PIC1_ICW4, PIC_ICW4_8086);

    // 禁止所有中断, 允许从PIC1传来的中断
    outb(PIC0_IMR, 0xFF & ~(1 << 2));
    outb(PIC1_IMR, 0xFF);
}

void irq_enable(int irq_num) {
    if (irq_num < IRQ_PIC_START) {
        return;
    }

    irq_num -= IRQ_PIC_START;
    if (irq_num < 8) {
        uint8_t mask = inb(PIC0_IMR) & ~(1 << irq_num);
        outb(PIC0_IMR, mask);
    } else {
        irq_num -= 8;
        uint8_t mask = inb(PIC1_IMR) & ~(1 << irq_num);
        outb(PIC1_IMR, mask);
    }
}

void irq_disable(int irq_num) {
    if (irq_num < IRQ_PIC_START) {
        return;
    }

    irq_num -= IRQ_PIC_START;
    if (irq_num < 8) {
        uint8_t mask = inb(PIC0_IMR) | (1 << irq_num);
        outb(PIC0_IMR, mask);
    } else {
        irq_num -= 8;
        uint8_t mask = inb(PIC1_IMR) | (1 << irq_num);
        outb(PIC1_IMR, mask);
    }
}

void irq_disable_global(void) { cli(); }

void irq_enable_global(void) { sti(); }
