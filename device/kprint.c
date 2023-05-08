#include "../include/kprint.h"
#include "../include/asm.h"
#include "../include/klib.h"

// void kprint(const char *fmt, ...) { kprintf(fmt); }

// /**
//  * @brief 日志打印
//  */
// void kprintf(const char *fmt, ...) {
//     char str_buf[128];
//     va_list args;

//     kernel_memset(str_buf, '\0', sizeof(str_buf));

//     va_start(args, fmt);
//     kernel_vsprintf(str_buf, fmt, args);
//     va_end(args);

//     disp_str(str_buf);
// }

// void kprintln(const char *fmt, ...) {
//     kprintf(fmt);
//     disp_str("\n");
//     disp_pos += screen_col;
// }

// void spanic(const char *file, int line, const char *func, const char *cond) {
//     kprintf("assert failed! %s", cond);
//     kprintf("file: %s\nline %d\nfunc: %s\n", file, line, func);

//     for (;;) {
//         hlt();
//     }
// }
