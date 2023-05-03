// /* 中断向量 */
// #define INT_VECTOR_DIVIDE 0x0
// #define INT_VECTOR_DEBUG 0x1
// #define INT_VECTOR_NMI 0x2
// #define INT_VECTOR_BREAKPOINT 0x3
// #define INT_VECTOR_OVERFLOW 0x4
// #define INT_VECTOR_BOUNDS 0x5
// #define INT_VECTOR_INVAL_OP 0x6
// #define INT_VECTOR_COPROC_NOT 0x7
// #define INT_VECTOR_DOUBLE_FAULT 0x8
// #define INT_VECTOR_COPROC_SEG 0x9
// #define INT_VECTOR_INVAL_TSS 0xA
// #define INT_VECTOR_SEG_NOT 0xB
// #define INT_VECTOR_STACK_FAULT 0xC
// #define INT_VECTOR_PROTECTION 0xD
// #define INT_VECTOR_PAGE_FAULT 0xE
// #define INT_VECTOR_COPROC_ERR 0x10

// /* 中断向量 */
// #define INT_VECTOR_IRQ0 0x20
// #define INT_VECTOR_IRQ8 0x28

// #define INT_M_CTL 0x20     // I/O port for interrupt controller       <Master>
// #define INT_M_CTLMASK 0x21 // setting bits in this port disables ints <Master>
// #define INT_S_CTL 0xA0     // I/O port for second interrupt controller<Slave>
// #define INT_S_CTLMASK 0xA1 // setting bits in this port disables ints <Slave>/

// #define DA_386IGate 0x8E /* 386 中断门类型值			*/

// #define PRIVILEGE_KRNL 0
// #define PRIVILEGE_TASK 1
// #define PRIVILEGE_USER 3

// typedef void (*int_handler)();

// typedef unsigned char u8;
// typedef unsigned short u16;
// typedef unsigned int u32;
// typedef unsigned long u64;

// /* 门描述符 */
// typedef struct s_gate {
//     u16 offset_low;  /* Offset Low */
//     u16 selector;    /* Selector */
//     u8 dcount;       /* 该字段只在调用门描述符中有效。如果在利用
//                         调用门调用子程序时引起特权级的转换和堆栈
//                         的改变，需要将外层堆栈中的参数复制到内层
//                         堆栈。该双字计数字段就是用于说明这种情况
//                         发生时，要复制的双字参数的数量。*/
//     u8 attr;         /* P(1) DPL(2) DT(1) TYPE(4) */
//     u16 offset_high; /* Offset High */
// } GATE;

// #define SELECTOR_KERNEL_CS 0x8
// #define SELECTOR_KERNEL_DS 0x10

// /* 中断处理函数 */
// void divide_error();
// void single_step_exception();
// void nmi();
// void breakpoint_exception();
// void overflow();
// void bounds_check();
// void inval_opcode();
// void copr_not_available();
// void double_fault();
// void copr_seg_overrun();
// void inval_tss();
// void segment_not_present();
// void stack_exception();
// void general_protection();
// void page_fault();
// void copr_error();

// void out_byte(int, int);

// extern u8 idt_ptr[6];

// extern GATE IDT_Table[256];

// // /*======================================================================*
// //                              init_idt_desc
// //  *----------------------------------------------------------------------*
// //  初始化 386 中断门
// //  *======================================================================*/
// // void init_idt_desc(unsigned char vector, u8 desc_type, int_handler handler, unsigned char privilege) {
// //     GATE *p_gate = &IDT_Table[vector];
// //     u32 base = (u32)handler;
// //     p_gate->offset_low = base & 0xFFFF;
// //     p_gate->selector = SELECTOR_KERNEL_CS;
// //     p_gate->dcount = 0;
// //     p_gate->attr = desc_type | (privilege << 5);
// //     p_gate->offset_high = (base >> 16) & 0xFFFF;
// // }

// // /*======================================================================*
// //                             init_prot
// //  *======================================================================*/
// // void init_prot() {
// //     // 全部初始化成中断门(没有陷阱门)
// //     init_idt_desc(INT_VECTOR_DIVIDE, DA_386IGate, divide_error, PRIVILEGE_KRNL);

// //     init_idt_desc(INT_VECTOR_DEBUG, DA_386IGate, single_step_exception, PRIVILEGE_KRNL);

// //     init_idt_desc(INT_VECTOR_NMI, DA_386IGate, nmi, PRIVILEGE_KRNL);

// //     init_idt_desc(INT_VECTOR_BREAKPOINT, DA_386IGate, breakpoint_exception, PRIVILEGE_USER);

// //     init_idt_desc(INT_VECTOR_OVERFLOW, DA_386IGate, overflow, PRIVILEGE_USER);

// //     init_idt_desc(INT_VECTOR_BOUNDS, DA_386IGate, bounds_check, PRIVILEGE_KRNL);

// //     init_idt_desc(INT_VECTOR_INVAL_OP, DA_386IGate, inval_opcode, PRIVILEGE_KRNL);

// //     init_idt_desc(INT_VECTOR_COPROC_NOT, DA_386IGate, copr_not_available, PRIVILEGE_KRNL);

// //     init_idt_desc(INT_VECTOR_DOUBLE_FAULT, DA_386IGate, double_fault, PRIVILEGE_KRNL);

// //     init_idt_desc(INT_VECTOR_COPROC_SEG, DA_386IGate, copr_seg_overrun, PRIVILEGE_KRNL);

// //     init_idt_desc(INT_VECTOR_INVAL_TSS, DA_386IGate, inval_tss, PRIVILEGE_KRNL);

// //     init_idt_desc(INT_VECTOR_SEG_NOT, DA_386IGate, segment_not_present, PRIVILEGE_KRNL);

// //     init_idt_desc(INT_VECTOR_STACK_FAULT, DA_386IGate, stack_exception, PRIVILEGE_KRNL);

// //     init_idt_desc(INT_VECTOR_PROTECTION, DA_386IGate, general_protection, PRIVILEGE_KRNL);

// //     init_idt_desc(INT_VECTOR_PAGE_FAULT, DA_386IGate, page_fault, PRIVILEGE_KRNL);

// //     init_idt_desc(INT_VECTOR_COPROC_ERR, DA_386IGate, copr_error, PRIVILEGE_KRNL);
// // }

// // /*======================================================================*
// //                             exception_handler
// //  *----------------------------------------------------------------------*
// //  异常处理
// //  *======================================================================*/
// // void exception_handler(int vec_no, int err_code, int eip, int cs, int eflags) {
// //     int *addr = (int *)0xb8000;
// //     // int i;

// //     *((char *)addr + 1024) = (char)0xf0;

// //     // int disp_pos = 0;

// //     // int i;
// //     // int text_color = 0x74; /* 灰底红字 */

// //     // char *err_msg[] = {"#DE Divide Error",
// //     //                    "#DB RESERVED",
// //     //                    "--  NMI Interrupt",
// //     //                    "#BP Breakpoint",
// //     //                    "#OF Overflow",
// //     //                    "#BR BOUND Range Exceeded",
// //     //                    "#UD Invalid Opcode (Undefined Opcode)",
// //     //                    "#NM Device Not Available (No Math Coprocessor)",
// //     //                    "#DF Double Fault",
// //     //                    "    Coprocessor Segment Overrun (reserved)",
// //     //                    "#TS Invalid TSS",
// //     //                    "#NP Segment Not Present",
// //     //                    "#SS Stack-Segment Fault",
// //     //                    "#GP General Protection",
// //     //                    "#PF Page Fault",
// //     //                    "--  (Intel reserved. Do not use.)",
// //     //                    "#MF x87 FPU Floating-Point Error (Math Fault)",
// //     //                    "#AC Alignment Check",
// //     //                    "#MC Machine Check",
// //     //                    "#XF SIMD Floating-Point Exception"};

// //     // /* 通过打印空格的方式清空屏幕的前五行，并把 disp_pos 清零 */
// //     // disp_pos = 0;
// //     // for (i = 0; i < 80 * 5; i++) {
// //     //     disp_str(" ");
// //     // }
// //     // disp_pos = 0;

// //     // disp_color_str("Exception! --> ", text_color);
// //     // disp_color_str(err_msg[vec_no], text_color);
// //     // disp_color_str("\n\n", text_color);
// //     // disp_color_str("EFLAGS:", text_color);
// //     // disp_int(eflags);
// //     // disp_color_str("CS:", text_color);
// //     // disp_int(cs);
// //     // disp_color_str("EIP:", text_color);
// //     // disp_int(eip);

// //     // if (err_code != 0xFFFFFFFF) {
// //     //     disp_color_str("Error code:", text_color);
// //     //     disp_int(err_code);
// //     // }
// // }

// // 交换两个数字的函数
// void swap(char *x, char *y) {
//     char t = *x;
//     *x = *y;
//     *y = t;
// }

// // 反转 `buffer [i…j]` 的函数
// char *reverse(char *buffer, int i, int j) {
//     while (i < j) {
//         swap(&buffer[i++], &buffer[j--]);
//     }

//     return buffer;
// }

// int abs(int a) {
//     if (a >= 0) {
//         return a;
//     }
//     return -a;
// }

// // 在 C 中实现 `itoa ()` 函数的迭代函数
// char *itoa(int value, char *buffer, int base) {
//     // 输入无效
//     if (base < 2 || base > 32) {
//         return buffer;
//     }

//     // 考虑数字的绝对值
//     int n = abs(value);

//     int i = 0;
//     while (n) {
//         int r = n % base;

//         if (r >= 10) {
//             buffer[i++] = 65 + (r - 10);
//         } else {
//             buffer[i++] = 48 + r;
//         }

//         n = n / base;
//     }

//     // 如果数字为 0
//     if (i == 0) {
//         buffer[i++] = '0';
//     }

//     // 如果基数为 10 且值为负数，则结果字符串
//     // 前面有一个减号 (-)
//     // 对于任何其他基数，值总是被认为是无符号的
//     if (value < 0 && base == 10) {
//         buffer[i++] = '-';
//     }

//     buffer[i] = '\0'; // null 终止字符串

//     // 反转字符串并返回
//     return reverse(buffer, 0, i - 1);
// }

// #include <stdint.h>

// // GDT 描述符结构体
// typedef struct {
//     uint16_t limit_low;  // gdt 限长低 16 位
//     uint16_t base_low;   // gdt 起始地址低 16 位
//     uint8_t base_middle; // gdt 起始地址中间 8 位
//     uint8_t access;      // 访问标志
//     uint8_t granularity; // 段界限高 4 位和其它标志位
//     uint8_t base_high;   // gdt 起始地址高 8 位
// } __attribute__((packed)) gdt_entry_t;

// // GDT 指针结构体
// typedef struct {
//     uint16_t limit; // gdt 限长
//     uint64_t base;  // gdt 起始地址
// } __attribute__((packed)) gdt_ptr_t;

// // 定义 GDT 描述符数组
// extern gdt_entry_t GDT_Table[];

// // 定义 GDT 指针
// extern gdt_ptr_t GDT_POINTER;

// void print(int a, int b, char *str);

void kernel_main(void) {
    int *add = (int *)0xb8000;
    *((char *)add) = (char)0xf0;

    // // 获取 GDT 说明信息
    // uint16_t size = GDT_POINTER.limit + 1;
    // uint64_t addr = GDT_POINTER.base;

    // // // 遍历 GDT 数组，打印每个描述符的字段值
    // // for (uint16_t i = 0; i < size / sizeof(gdt_entry_t); i++) {
    // //     gdt_entry_t *entry = &GDT_Table[i];
    // //     printf("Descriptor %d:\n", i);
    // //     printf("    limit_low:     0x%x\n", entry->limit_low);
    // //     printf("    base_low:      0x%x\n", entry->base_low);
    // //     printf("    base_middle:   0x%x\n", entry->base_middle);
    // //     printf("    access:        0x%x\n", entry->access);
    // //     printf("    granularity:   0x%x\n", entry->granularity);
    // //     printf("    base_high:     0x%x\n", entry->base_high);
    // // }

    // // print(11, 0, "enter kernel main");
    // // print(14, 0, "asdfasdf");

    // char buffer[65];
    // itoa(addr, buffer, 16);
    // print(11, 0, buffer);

    *((char *)add + 1024) = (char)0xf0;

    // int i = 1 / 0;
    while (1)
        ;
}
