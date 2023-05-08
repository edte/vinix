
#ifndef __BUFFER_H
#define __BUFFER_H

#include "type.h"

typedef struct  {
	byte * data;
	int size;				// 最大字节数
	int read, write;		// 当前读写位置
	int count;				// 当前已有的数据量
}buffer_t;

void buffer_init (buffer_t * buf, byte * data, int size);
int buffer_get (buffer_t * buf, byte * c);
int buffer_put (buffer_t * buf, byte c);



typedef struct  {
	int * data;
	int size;				// 最大字节数
	int read, write;		// 当前读写位置
	int count;				// 当前已有的数据量
}buffer32_t;

void buffer32_init (buffer32_t * buf, int * data, int size);
int buffer32_get (buffer32_t * buf, int * c);
int buffer32_put (buffer32_t * buf, int c);


#endif
