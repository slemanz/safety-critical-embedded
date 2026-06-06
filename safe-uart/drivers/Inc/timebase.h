#ifndef INC_TIMEBASE_H
#define INC_TIMEBASE_H

#include <stdint.h>

uint32_t tick_get(void);
void tick_delay(uint32_t delay);
void timebase_init(void);

#endif /* INC_TIMEBASE_H */