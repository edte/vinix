#ifndef TIMER_H
#define TIMER_H

#include "type.h"

extern uint64_t sys_tick; // 系统启动后的tick数量

#define PIT_OSC_FREQ 1193182 // 定时器时钟
#define OS_TICK_MS 10        // 每毫秒的时钟数

// 定时器的寄存器和各项位配置
#define PIT_CHANNEL0_DATA_PORT 0x40
#define PIT_COMMAND_MODE_PORT 0x43

// 控制信号
#define PIT_CHANNLE0 (0 << 6)
#define PIT_LOAD_LOHI (3 << 4)
#define PIT_MODE0 (3 << 1)

// 频率
#define HZ 100

#define millisecond 1
#define second 1000
#define minute second*60
#define hour minute*60
#define day hour*60
#define week day*7
#define year day*315

void init_time(void);

u64 get_ticks();

void milli_delay(int milli_sec);

#endif // OS_TIMER_H
