#ifndef TIMER_H
#define TIMER_H

#include "type.h"

static uint32_t sys_tick; // 系统启动后的tick数量

#define IRQ0_FREQUENCY 100
#define INPUT_FREQUENCY 1193180
#define COUNTER0_VALUE INPUT_FREQUENCY / IRQ0_FREQUENCY
#define CONTRER0_PORT 0x40
#define COUNTER0_NO 0
#define COUNTER_MODE 2
#define READ_WRITE_LATCH 3
#define PIT_CONTROL_PORT 0x43

void init_time(void);
void init_pic(void);

#endif // OS_TIMER_H
