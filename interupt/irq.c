#include "../include/irq.h"
#include "../include/asm.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/kprint.h"
#include "../include/time.h"

static void dump_core_regs(irq_frame_t *frame) {
    kprintln("rax:0x%x", frame->rax);
    kprintln("rbx:0x%x", frame->rbx);
    kprintln("rcx:0x%x", frame->rcx);
    kprintln("rdx:0x%x", frame->rdx);
    kprintln("rbp:0x%x", frame->rbp);
    kprintln("rsp:0x%x", frame->rsp);
    kprintln("rsi:0x%x", frame->rsi);
    kprintln("rdi:0x%x", frame->rdi);
    kprintln("r8:0x%x", frame->r8);
    kprintln("r9:0x%x", frame->r9);
    kprintln("r10:0x%x", frame->r10);
    kprintln("r11:0x%x", frame->r11);
    kprintln("r12:0x%x", frame->r12);
    kprintln("r13:0x%x", frame->r13);
    kprintln("r14:0x%x", frame->r14);
    kprintln("r15:0x%x", frame->r15);
    kprintln("cs:0x%x", frame->cs);
    kprintln("rflags:0x%x", frame->rflags);
}

static void do_default_handler(irq_frame_t *frame, const char *message) {
    // disp_pos = 0;
    kprintln("");
    kprintln("-------------------------------------------------------");
    kprintln("IRQ/Exception happend: %s", message);
    dump_core_regs(frame);

    // todo: 留等以后补充打印任务栈的内容

    kprintln("-------------------------------------------------------");
    for (;;) {
        hlt();
    }
}

extern void do_handler_unknown(irq_frame_t *frame) { do_default_handler(frame, "Unknown exception."); }
extern void do_handler_divider(irq_frame_t *frame) { do_default_handler(frame, "Divider Error."); }
// extern void do_handler_divider(irq_frame_t *frame) { kprintln("divide 0"); }
extern void do_handler_Debug(irq_frame_t *frame) { do_default_handler(frame, "Debug Exception"); }
extern void do_handler_NMI(irq_frame_t *frame) { do_default_handler(frame, "NMI Interrupt."); }
extern void do_handler_breakpoint(irq_frame_t *frame) { do_default_handler(frame, "Breakpoint."); }
extern void do_handler_overflow(irq_frame_t *frame) { do_default_handler(frame, "Overflow."); }
extern void do_handler_bound_range(irq_frame_t *frame) { do_default_handler(frame, "BOUND Range Exceeded."); }
extern void do_handler_invalid_opcode(irq_frame_t *frame) { do_default_handler(frame, "Invalid Opcode."); }
extern void do_handler_device_unavailable(irq_frame_t *frame) { do_default_handler(frame, "Device Not Available."); }
extern void do_handler_double_fault(irq_frame_t *frame) { do_default_handler(frame, "Double Fault."); }
extern void do_handler_invalid_tss(irq_frame_t *frame) { do_default_handler(frame, "Invalid TSS"); }
extern void do_handler_segment_not_present(irq_frame_t *frame) { do_default_handler(frame, "Segment Not Present."); }
extern void do_handler_stack_segment_fault(irq_frame_t *frame) { do_default_handler(frame, "Stack-Segment Fault."); }
extern void do_handler_general_protection(irq_frame_t *frame) { do_default_handler(frame, "General Protection."); }
extern void do_handler_page_fault(irq_frame_t *frame) { do_default_handler(frame, "Page Fault."); }
extern void do_handler_fpu_error(irq_frame_t *frame) { do_default_handler(frame, "X87 FPU Floating Point Error."); }
extern void do_handler_alignment_check(irq_frame_t *frame) { do_default_handler(frame, "Alignment Check."); }
extern void do_handler_machine_check(irq_frame_t *frame) { do_default_handler(frame, "Machine Check."); }
extern void do_handler_smd_exception(irq_frame_t *frame) { do_default_handler(frame, "SIMD Floating Point Exception."); }
extern void do_handler_virtual_exception(irq_frame_t *frame) { do_default_handler(frame, "Virtualization Exception."); }

extern void do_handler_timer() {
    sys_tick++;
    kprintln("%d", sys_tick);
};

extern void do_handler_keyboard() {
    outb(0x20, 0x61);
    uint16_t scancode = inb(0x60);
    kprintln("");
    kprintln("%d", scancode);
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
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

// 开某个中断
void irq_enable(int irq_num) {
    if (irq_num < PIC_M_CTRL) {
        return;
    }

    irq_num -= PIC_M_CTRL;
    if (irq_num < 8) {
        uint8_t mask = inb(PIC_M_DATA) & ~(1 << irq_num);
        outb(PIC_M_DATA, mask);
    } else {
        irq_num -= 8;
        uint8_t mask = inb(PIC_S_DATA) & ~(1 << irq_num);
        outb(PIC_S_DATA, mask);
    }
}

// 关某个中断
void irq_disable(int irq_num) {
    if (irq_num < PIC_M_CTRL) {
        return;
    }

    irq_num -= PIC_M_CTRL;
    if (irq_num < 8) {
        uint8_t mask = inb(PIC_M_DATA) | (1 << irq_num);
        outb(PIC_M_DATA, mask);
    } else {
        irq_num -= 8;
        uint8_t mask = inb(PIC_S_DATA) | (1 << irq_num);
        outb(PIC_S_DATA, mask);
    }
}

// 开启所有中断
void irq_enable_global(void) { sti(); }

// 关闭所有中断
void irq_disable_global(void) { cli(); }
