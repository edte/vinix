#include "../include/kprint.h"
#include "../include/asm.h"
#include "../include/klib.h"

void kprint(const char *fmt, ...) { kprintf(fmt); }

/**
 * @brief 日志打印
 */
void kprintf(const char *fmt, ...) {
    char str_buf[128];
    va_list args;

    kernel_memset(str_buf, '\0', sizeof(str_buf));

    va_start(args, fmt);
    kernel_vsprintf(str_buf, fmt, args);
    va_end(args);

    if (disp_pos >= 30) {
        disp_pos = 0;
    }

    disp_str_raw(disp_pos, 0, str_buf);
    disp_pos++;

    // const char *p = str_buf;
    // while (*p != '\0') {
    //     while ((inb(COM1_PORT + 5) & (1 << 6)) == 0)
    //         ;
    //     outb(COM1_PORT, *p++);
    // }

    // outb(COM1_PORT, '\r');
    // outb(COM1_PORT, '\n');
}

void kprintln(const char *fmt, ...) {
    disp_pos++;
    kprintf(fmt);
}

void spanic(const char *file, int line, const char *func, const char *cond) {
    kprintf("assert failed! %s", cond);
    kprintf("file: %s\nline %d\nfunc: %s\n", file, line, func);

    for (;;) {
        hlt();
    }
}
