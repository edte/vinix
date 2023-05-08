#include "../include/time.h"
#include "../include/asm.h"
#include "../include/idt.h"
#include "../include/irq.h"

/* 初始化PIT8253 */
void init_time() {
    uint32_t reload_count = PIT_OSC_FREQ / (1000.0 / OS_TICK_MS);

    outb(PIT_COMMAND_MODE_PORT, PIT_CHANNLE0 | PIT_LOAD_LOHI | PIT_MODE0);
    outb(PIT_CHANNEL0_DATA_PORT, reload_count & 0xFF);        // 加载低8位
    outb(PIT_CHANNEL0_DATA_PORT, (reload_count >> 8) & 0xFF); // 再加载高8位
}

u64 get_ticks() { return sys_tick; }

void milli_delay(int milli_sec) {
    int t = get_ticks();

    while (((get_ticks() - t) * 1000 / HZ) < milli_sec) {
    }
}
