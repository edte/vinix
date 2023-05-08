#include "../include/irq.h"
#include "../include/keyboard.h"
#include "../include/kprint.h"
#include "../include/tty.h"
#include "../include/buffer.h"

int tty_in_buf[TTY_IN_BYTES];
int tty_out_buf[TTY_OUT_BYTES];

void task_tty()
{
	tty_t*	p_tty;

	init_keyboard();
	init_tty(p_tty);

    while(1) {
    	tty_do_read(p_tty);
    	tty_do_write(p_tty);
    }
}


void init_tty(tty_t* p_tty)
{
    buffer32_init(&p_tty->in_buffer, tty_in_buf,TTY_IN_BYTES);
    buffer32_init(&p_tty->out_buffer, tty_out_buf,TTY_OUT_BYTES);
}


void tty_do_read(tty_t* p_tty)
{
	keyboard_read(p_tty);
}

void tty_do_write(tty_t* p_tty)
{
    int c;

    while (p_tty->out_buffer.count) {
        // kprintf("begin write tty\n");
        buffer32_get(&p_tty->out_buffer, &c);
        kprintf("%c",c);
    }    
}


void in_process(tty_t* tty, u32 key)
{
    // kprintf("enter process, key:%d\n", key);

	if (!(key & FLAG_EXT)) {
        // kprintf("begin put key\n");
		put_key(tty, key);
        return;
	}

	int raw_code = key & MASK_RAW;
	switch(raw_code) {
	case ENTER:
		put_key(tty, '\n');
		break;
	case BACKSPACE:
		put_key(tty, '\b');
		break;
	case UP:
		if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {	/* Shift + Up */
			// scroll_screen(tty->p_console, SCR_DN);
		}
		break;
	case DOWN:
		if ((key & FLAG_SHIFT_L) ||   (key & FLAG_SHIFT_R)) {	/* Shift + Down */
			// scroll_screen(tty->p_console, SCR_UP);
		}
		break;
	case F1:
	case F2:
	case F3:
	case F4:
	case F5:
	case F6:
	case F7:
	case F8:
	case F9:
	case F10:
	case F11:
	case F12:
		if ((key & FLAG_ALT_L) || (key & FLAG_ALT_R)) {	/* Alt + F1~F12 */
			// select_console(raw_code - F1);
		}
		break;
	default:
		break;
	}
}

void put_key(tty_t* p_tty, u32 key)
{
    byte c;

    buffer32_put(&p_tty->out_buffer, key);
}
