#include "../include/irq.h"
#include "../include/asm.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/sprintf.h"

void exception_handler_unknown() { disp_str_raw(10, 0, "Unkown interrupt occurred"); }

// 除 0 异常程序
void exception_handler_divider() { disp_str_raw(10, 0, "Devide Error"); }

void exception_handler_Debug(void) { disp_str_raw(10, 0, "Debug exception"); };
void exception_handler_NMI(void) { disp_str_raw(10, 0, "NMI exception"); };
void exception_handler_breakpoint(void){};
void exception_handler_overflow(void){};
void exception_handler_bound_range(void){};
void exception_handler_invalid_opcode(void){};
void exception_handler_device_unavailable(void){};
void exception_handler_double_fault(void){};
void exception_handler_invalid_tss(void){};
void exception_handler_segment_not_present(void){};
void exception_handler_stack_segment_fault(void){};
void exception_handler_general_protection(void){};
void exception_handler_page_fault(void){};
void exception_handler_fpu_error(void){};
void exception_handler_alignment_check(void){};
void exception_handler_machine_check(void){};
void exception_handler_smd_exception(void){};
void exception_handler_virtual_exception(void){};

void init_irq() {
    // 初始化所有异常
    for (u32 i = 0; i < IDT_TABLE_SIZE; i++) {
        irq_install(i, exception_handler_unknown);
    }

    // 设置其他异常处理
    irq_install(IRQ0_DE, exception_handler_divider);
    irq_install(IRQ1_DB, exception_handler_Debug);
    irq_install(IRQ2_NMI, exception_handler_NMI);
    irq_install(IRQ3_BP, exception_handler_breakpoint);
    irq_install(IRQ4_OF, exception_handler_overflow);
    irq_install(IRQ5_BR, exception_handler_bound_range);
    irq_install(IRQ6_UD, exception_handler_invalid_opcode);
    irq_install(IRQ7_NM, exception_handler_device_unavailable);
    irq_install(IRQ8_DF, exception_handler_double_fault);
    irq_install(IRQ10_TS, exception_handler_invalid_tss);
    irq_install(IRQ11_NP, exception_handler_segment_not_present);
    irq_install(IRQ12_SS, exception_handler_stack_segment_fault);
    irq_install(IRQ13_GP, exception_handler_general_protection);
    irq_install(IRQ14_PF, exception_handler_page_fault);
    irq_install(IRQ16_MF, exception_handler_fpu_error);
    irq_install(IRQ17_AC, exception_handler_alignment_check);
    irq_install(IRQ18_MC, exception_handler_machine_check);
    irq_install(IRQ19_XM, exception_handler_smd_exception);
    irq_install(IRQ20_VE, exception_handler_virtual_exception);
}

// 安装中断或异常处理程序
int irq_install(int irq_num, irq_handler_t handler) {
    if (irq_num >= IDT_TABLE_SIZE) {
        return -1;
    }

    init_idt_entry(idt_table + irq_num, (u64)SELECTOR_KERNEL_CODE, handler, 0,
                   GATE_P_PRESENT | GATE_DPL0 | GATE_TYPE_INTERRUPT);

    return 0;
}

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
