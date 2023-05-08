#ifndef _ORANGES_LOG_H_
#define _ORANGES_LOG_H_

// 清屏
void clear();

int kprintf(const char *fmt, ...);
int kprintln(const char *fmt, ...);

int kprintf_color(unsigned int FRcolor, unsigned int BKcolor, const char *fmt, ...);
int kprintln_color(unsigned int FRcolor, unsigned int BKcolor, const char *fmt, ...);

void putchar(unsigned int *fb, int Xsize, int x, int y, unsigned int FRcolor, unsigned int BKcolor, unsigned char font);

void init_screen();

#endif /* _ORANGES_LOG_H_ */
