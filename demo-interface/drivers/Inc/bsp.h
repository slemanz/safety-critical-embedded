#ifndef INC_BSP_H
#define INC_BSP_H

#include <stdint.h>
#include <stdbool.h>

void led_init(void);
void led_on(void);
void led_off(void);
void led_toggle(void);
void button_init(void);
bool button_get_state(void);

#endif /* INC_BSP_H */