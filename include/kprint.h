#ifndef _ORANGES_LOG_H_
#define _ORANGES_LOG_H_

// // 光标偏移左上角的位置（不是相对于显存基址），就是从 0 开始
// extern int disp_pos = 0;

// #define screen_row 25
// #define screen_col 160
// #define screen_base 0xb8000

// void kprint(const char *fmt, ...);
// void kprintf(const char *fmt, ...);
// void kprintln(const char *fmt, ...);

// void kpanic(const char *file, int line, const char *func, const char *cond);
// void kpanicf(const char *file, int line, const char *func, const char *cond);
// void kpanicln(const char *file, int line, const char *func, const char *cond);

// void disp_str_color(char *info, int color);
// void disp_str(char *info);
// void clear();

// void disp_str_raw(int a, int b, char *str);

#define STR_DEFAULT_LEN 1024

#endif /* _ORANGES_LOG_H_ */
