#ifndef __R_I2C_H__
#define __R_I2C_H__	 


#include "air32f10x.h"
#include "air32f10x_gpio.h"
#include "delay.h"



#define PROBE_SCL_Port                 GPIOB
#define PROBE_SCL_Pin                  GPIO_Pin_6
#define PROBE_SCL_SET                  GPIO_SetBits  (PROBE_SCL_Port, PROBE_SCL_Pin)
#define PROBE_SCL_RESET                GPIO_ResetBits(PROBE_SCL_Port, PROBE_SCL_Pin)

#define PROBE_SDA_Port                 GPIOB
#define PROBE_SDA_Pin                  GPIO_Pin_7
#define PROBE_SDA_SET                  GPIO_SetBits  (PROBE_SDA_Port, PROBE_SDA_Pin)
#define PROBE_SDA_RESET                GPIO_ResetBits(PROBE_SDA_Port, PROBE_SDA_Pin)
#define Read_PROBE_SDA                 GPIO_ReadInputDataBit(PROBE_SDA_Port, PROBE_SDA_Pin)



#define AT24MACxx_CLK_us               4//时钟线延时 4.12K的上拉电阻，实测上升沿时间为2.4us



void I2C_IO_Init(void);
uint8_t OLED_Data(uint8_t dev_addr,uint8_t address,uint8_t data);

#endif
