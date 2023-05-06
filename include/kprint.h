#ifndef _ORANGES_LOG_H_
#define _ORANGES_LOG_H_

void kprint(const char *fmt, ...);
void kprintf(const char *fmt, ...);
void kprintln(const char *fmt, ...);

void kpanic(const char *file, int line, const char *func, const char *cond);
void kpanicf(const char *file, int line, const char *func, const char *cond);
void kpanicln(const char *file, int line, const char *func, const char *cond);

void disp_str_raw(int a, int b, char *str);
void clear();

static int disp_pos = 0;

#define STR_DEFAULT_LEN 1024

#endif /* _ORANGES_LOG_H_ */
