
#ifndef __GPIO_H__
#define __GPIO_H__

#include "air32f10x.h"
#include "air32f10x_gpio.h"

void GPIO_Configuration(void);
void GPIO_Turn(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void battery_two(uint8_t value);
void battery_three(uint8_t value);
void battery_four(uint8_t value);
void battery_close(void);
void battery_select(uint8_t key);
#endif 


