#include "../include/idt.h"
#include "../include/asm.h"
#include "../include/gdt.h"
#include "../include/irq.h"
#include "../include/type.h"

// 设置门描述符
void init_idt_entry(idt_entry_t *p_entry, uint16_t selector, uint64_t offset, uint8_t ist, uint8_t type_attr) {
    p_entry->offset_low = (u16)offset;
    p_entry->offset_middle = (u16)(offset >> 16);
    p_entry->offset_high = (u32)(offset >> 32);
    p_entry->selector = selector;
    p_entry->ist = ist;
    p_entry->type_attr = type_attr;
    p_entry->reserved = 0;
}

// 安装中断或异常处理程序
void irq_install(int irq_num, irq_handler_t handler) {
    if (irq_num >= IDT_TABLE_SIZE) {
        return;
    }

    init_idt_entry(idt_table + irq_num, SELECTOR_KERNEL_CODE, (u64)handler, 0,
                   GATE_P_PRESENT | GATE_DPL0 | GATE_TYPE_INTERRUPT);

    return;
}

void init_idt(void) {
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

    // 设置指针
    idt_pointer.base = (u64)idt_table;
    idt_pointer.limit = sizeof(idt_table) - 1;

    // 加载
    lidt();

    // 初始化pic 控制器
    init_pic();
}
