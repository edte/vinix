#ifndef _ORANGES_STDIO_H_
#define _ORANGES_STDIO_H_

#include "type.h"

void print(int a, int b, char *str);
void clear();
void out_byte(u16 port, u8 value);
u8 in_byte(u16 port);

#endif /* _ORANGES_STDIO_H_ */
