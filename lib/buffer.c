#include "../include/buffer.h"
#include "../include/type.h"

// buffer初始化
void buffer_init (buffer_t * buf, byte * data, int size){
	buf->data = data;
	buf->count = 0;
	buf->size = size;
	buf->read = buf->write = 0;
}

// 取一字节数据    
int buffer_get (buffer_t * buf, byte * c){
	if (buf->count <= 0) {
		return -1;
	}

	*c = buf->data[buf->read++];
	if (buf->read >= buf->size) {
		buf->read = 0;
	}
	buf->count--;
	return 0;
}

// 写一字节数据
int buffer_put (buffer_t * buf, byte c) {
	if (buf->count >= buf->size) {
		return -1;
	}

	buf->data[buf->write++] = c;
	if (buf->write >= buf->size) {
		buf->write = 0;
	}
	buf->count++;

	return 0;
}



// buffer初始化
void buffer32_init (buffer32_t * buf, int * data, int size){
	buf->data = data;
	buf->count = 0;
	buf->size = size;
	buf->read = buf->write = 0;
}

// 取一字节数据    
int buffer32_get (buffer32_t * buf, int * c){
	if (buf->count <= 0) {
		return -1;
	}

	*c = buf->data[buf->read++];
	if (buf->read >= buf->size) {
		buf->read = 0;
	}
	buf->count--;
	return 0;
}

// 写一字节数据
int buffer32_put (buffer32_t * buf, int c) {
	if (buf->count >= buf->size) {
		return -1;
	}

	buf->data[buf->write++] = c;
	if (buf->write >= buf->size) {
		buf->write = 0;
	}
	buf->count++;

	return 0;
}
