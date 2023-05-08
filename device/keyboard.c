#include "../include/keyboard.h"
#include "../include/asm.h"
#include "../include/irq.h"
#include "../include/keymap.h"
#include "../include/tty.h"
#include "../include/type.h"
#include "../include/kprint.h"
#include "../include/buffer.h"

#define buffer_size 100
buffer_t kb_buf;
byte keyboard_data[buffer_size];
int key_pressed;
int code_with_E0;
int shift_l;    
int shift_r;    
int alt_l;      
int alt_r;      
int ctrl_l;     
int ctrl_r;     
int caps_lock;  
int num_lock;   
int scroll_lock;
int column;


void init_keyboard() {
    buffer_init(&kb_buf, keyboard_data, buffer_size);

    shift_l = 0;
    shift_r = 0;
    alt_l = 0;
    alt_r = 0;
    ctrl_l = 0;
    ctrl_r = 0;

    caps_lock = 0;
    num_lock = 1;
    scroll_lock = 0;

    column = 0;
}



byte get_byte_from_kbuf() {
	while (kb_buf.count <= 0) {} /* wait for a byte to arrive */

    byte scan_code;
    irq_disable_global();
    buffer_get(&kb_buf, &scan_code);
    irq_enable_global();
    return scan_code;
};


/*======================================================================*
                           keyboard_read
*======================================================================*/
void keyboard_read(tty_t* tty) {
    if (kb_buf.count<=0) {
        return;
    }

    // kprintf("begin read keyboard\n");

	u8	scan_code;

	/**
	 * 1 : make
	 * 0 : break
	 */
	int	make;

	/**
	 * We use a integer to record a key press.
	 * For instance, if the key HOME is pressed, key will be evaluated to
	 * `HOME' defined in keyboard.h.
	 */
	u32	key = 0;


	/**
	 * This var points to a row in keymap[]. I don't use two-dimension
	 * array because I don't like it.
	 */
	u32*	keyrow;

	while (kb_buf.count > 0) {
		code_with_E0 = 0;
		scan_code = get_byte_from_kbuf();

		/* parse the scan code below */
		if (scan_code == 0xE1) {
			int i;
			u8 pausebreak_scan_code[] = {0xE1, 0x1D, 0x45, 0xE1, 0x9D, 0xC5};
			int is_pausebreak = 1;
			for (i = 1; i < 6; i++) {
				if (get_byte_from_kbuf() != pausebreak_scan_code[i]) {
					is_pausebreak = 0;
					break;
				}
			}
			if (is_pausebreak) {
				key = PAUSEBREAK;
			}
		}
		else if (scan_code == 0xE0) {
			code_with_E0 = 1;
			scan_code = get_byte_from_kbuf();

			/* PrintScreen is pressed */
			if (scan_code == 0x2A) {
				code_with_E0 = 0;
				if ((scan_code = get_byte_from_kbuf()) == 0xE0) {
					code_with_E0 = 1;
					if ((scan_code = get_byte_from_kbuf()) == 0x37) {
						key = PRINTSCREEN;
						make = 1;
					}
				}
			}
			/* PrintScreen is released */
			else if (scan_code == 0xB7) {
				code_with_E0 = 0;
				if ((scan_code = get_byte_from_kbuf()) == 0xE0) {
					code_with_E0 = 1;
					if ((scan_code = get_byte_from_kbuf()) == 0xAA) {
						key = PRINTSCREEN;
						make = 0;
					}
				}
			}
		}

		if ((key != PAUSEBREAK) && (key != PRINTSCREEN)) {
			int caps;

			/* make or break */
			make = (scan_code & FLAG_BREAK ? 0 : 1);
			
			keyrow = &keymap[(scan_code & 0x7F) * MAP_COLS];

			column = 0;

			caps = shift_l || shift_r;
			if (caps_lock &&
			    keyrow[0] >= 'a' && keyrow[0] <= 'z')
				caps = !caps;

			if (caps)
				column = 1;

			if (code_with_E0)
				column = 2;

			key = keyrow[column];

			switch(key) {
			case SHIFT_L:
				shift_l	= make;
				break;
			case SHIFT_R:
				shift_r	= make;
				break;
			case CTRL_L:
				ctrl_l	= make;
				break;
			case CTRL_R:
				ctrl_r	= make;
				break;
			case ALT_L:
				alt_l	= make;
				break;
			case ALT_R:
				alt_l	= make;
				break;
			case CAPS_LOCK:
				if (make) {
					caps_lock   = !caps_lock;
				}
				break;
			case NUM_LOCK:
				if (make) {
				}
				break;
			case SCROLL_LOCK:
				if (make) {
					scroll_lock = !scroll_lock;
				}
				break;
			default:
				break;
			}
		}

		if(make){ /* Break Code is ignored */
			int pad = 0;

			/* deal with the numpad first */
			if ((key >= PAD_SLASH) && (key <= PAD_9)) {
				pad = 1;
				switch(key) {	/* '/', '*', '-', '+',
						 * and 'Enter' in num pad
						 */
				case PAD_SLASH:
					key = '/';
					break;
				case PAD_STAR:
					key = '*';
					break;
				case PAD_MINUS:
					key = '-';
					break;
				case PAD_PLUS:
					key = '+';
					break;
				case PAD_ENTER:
					key = ENTER;
					break;
				default:
					/* the value of these keys
					 * depends on the Numlock
					 */
					if (num_lock) {	/* '0' ~ '9' and '.' in num pad */
						if (key >= PAD_0 && key <= PAD_9)
							key = key - PAD_0 + '0';
						else if (key == PAD_DOT)
							key = '.';
					}
					else{
						switch(key) {
						case PAD_HOME:
							key = HOME;
							break;
						case PAD_END:
							key = END;
							break;
						case PAD_PAGEUP:
							key = PAGEUP;
							break;
						case PAD_PAGEDOWN:
							key = PAGEDOWN;
							break;
						case PAD_INS:
							key = INSERT;
							break;
						case PAD_UP:
							key = UP;
							break;
						case PAD_DOWN:
							key = DOWN;
							break;
						case PAD_LEFT:
							key = LEFT;
							break;
						case PAD_RIGHT:
							key = RIGHT;
							break;
						case PAD_DOT:
							key = DELETE;
							break;
						default:
							break;
						}
					}
					break;
				}
			}
			key |= shift_l	? FLAG_SHIFT_L	: 0;
			key |= shift_r	? FLAG_SHIFT_R	: 0;
			key |= ctrl_l	? FLAG_CTRL_L	: 0;
			key |= ctrl_r	? FLAG_CTRL_R	: 0;
			key |= alt_l	? FLAG_ALT_L	: 0;
			key |= alt_r	? FLAG_ALT_R	: 0;
			key |= pad	? FLAG_PAD	: 0;

			in_process(tty, key);
		}
	}
}
