#ifndef _ORANGES_ASM_H_
#define _ORANGES_ASM_H_

#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "type.h"

static inline u8 inb(u16 port) {
    u8 rv;
    __asm__ __volatile__("inb %[p], %[v]" : [v] "=a"(rv) : [p] "d"(port));
    return rv;
}

static inline u16 inw(u16 port) {
    u16 rv;
    __asm__ __volatile__("in %1, %0" : "=a"(rv) : "dN"(port));
    return rv;
}

static inline void outb(u16 port, u8 data) { __asm__ __volatile__("outb %[v], %[p]" : : [p] "d"(port), [v] "a"(data)); }

static inline void cli() { __asm__ __volatile__("cli"); }

static inline void sti() { __asm__ __volatile__("sti"); }

static inline void iretq() { __asm__ __volatile__("iretq"); }

static inline void lgdt() { __asm__ __volatile__("lgdt %[g]" ::[g] "m"(gdt_pointer)); }

static inline void lidt() { __asm__ __volatile__("lidt %[g]" ::[g] "m"(idt_pointer)); }

static inline u32 read_cr0() {
    u32 cr0;
    __asm__ __volatile__("mov %%cr0, %[v]" : [v] "=r"(cr0));
    return cr0;
}

static inline void write_cr0(u32 v) { __asm__ __volatile__("mov %[v], %%cr0" ::[v] "r"(v)); }

static inline uint32_t read_cr2() {
    uint32_t cr2;
    __asm__ __volatile__("mov %%cr2, %[v]" : [v] "=r"(cr2));
    return cr2;
}

static inline void write_cr3(uint32_t v) { __asm__ __volatile__("mov %[v], %%cr3" ::[v] "r"(v)); }

static inline uint32_t read_cr3() {
    uint32_t cr3;
    __asm__ __volatile__("mov %%cr3, %[v]" : [v] "=r"(cr3));
    return cr3;
}

static inline uint32_t read_cr4() {
    uint32_t cr4;
    __asm__ __volatile__("mov %%cr4, %[v]" : [v] "=r"(cr4));
    return cr4;
}

static inline void write_cr4(uint32_t v) { __asm__ __volatile__("mov %[v], %%cr4" ::[v] "r"(v)); }

static inline void write_es(u32 v) { __asm__ __volatile__("mov %[v], %%es" ::[v] "r"(v)); }
static inline void write_ds(u32 v) { __asm__ __volatile__("mov %[v], %%ds" ::[v] "r"(v)); }
static inline void write_gs(u32 v) { __asm__ __volatile__("mov %[v], %%gs" ::[v] "r"(v)); }
static inline void write_ss(u32 v) { __asm__ __volatile__("mov %[v], %%ss" ::[v] "r"(v)); }
static inline void write_fs(u32 v) { __asm__ __volatile__("mov %[v], %%fs" ::[v] "r"(v)); }

static inline void far_jump(u32 selector, u32 offset) {
    u32 addr[] = {offset, selector};
    __asm__ __volatile__("ljmpl *(%[a])" ::[a] "r"(addr));
}

static inline void hlt(void) { __asm__ __volatile__("hlt"); }

static inline void write_tr(uint32_t tss_selector) { __asm__ __volatile__("ltr %%ax" ::"a"(tss_selector)); }

static inline uint32_t read_eflags(void) {
    uint32_t eflags;

    __asm__ __volatile__("pushfl\n\tpopl %%eax" : "=a"(eflags));
    return eflags;
}

static inline void write_eflags(uint32_t eflags) { __asm__ __volatile__("pushl %%eax\n\tpopfl" ::"a"(eflags)); }

#endif /* _ORANGES_ASM_H_ */
