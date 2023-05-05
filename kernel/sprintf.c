#include "../include/sprintf.h"
#include "../include/asm.h"
#include "../include/klib.h"

/**
 * @brief 日志打印
 */
void sprintf(const char *fmt, ...) {
    char str_buf[128];
    va_list args;

    kernel_memset(str_buf, '\0', sizeof(str_buf));

    va_start(args, fmt);
    kernel_vsprintf(str_buf, fmt, args);
    va_end(args);

    disp_str_raw(1, 0, str_buf);

    // const char *p = str_buf;
    // while (*p != '\0') {
    //     while ((inb(COM1_PORT + 5) & (1 << 6)) == 0)
    //         ;
    //     outb(COM1_PORT, *p++);
    // }

    // outb(COM1_PORT, '\r');
    // outb(COM1_PORT, '\n');
}

void spanic(const char *file, int line, const char *func, const char *cond) {
    sprintf("assert failed! %s", cond);
    sprintf("file: %s\nline %d\nfunc: %s\n", file, line, func);

    for (;;) {
        hlt();
    }
}
