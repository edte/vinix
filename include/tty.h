#ifndef _ORANGES_TTY_H_
#define _ORANGES_TTY_H_

#include "../include/type.h"
#include "../include/buffer.h"

#define TTY_IN_BYTES		256	/* tty input queue size */
#define TTY_OUT_BYTES		256	/* tty output buffer size */


extern int tty_in_buf[TTY_IN_BYTES];
extern int tty_out_buff[TTY_OUT_BYTES];


/* TTY */
typedef struct
{
    buffer32_t in_buffer;
    buffer32_t out_buffer;
}tty_t;

void task_tty();
void in_process(tty_t* tty, u32 key);
void init_tty	(tty_t* tty);
void tty_write(tty_t* p_tty, char* buf, int len);
void tty_do_write(tty_t* p_tty);
void tty_do_read(tty_t* p_tty);
void put_key(tty_t* p_tty, u32 key);

#endif /* _ORANGES_TTY_H_ */
