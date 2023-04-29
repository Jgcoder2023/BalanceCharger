#ifndef __R_SPI_H__
#define __R_SPI_H__

#include "air32f10x.h"
#include "air32f10x_gpio.h"


/*使用什么模式就将对应宏定义改成1*/
#define 				SPI_Mode_0         1
#define 				SPI_Mode_1				 0
#define 				SPI_Mode_2				 0
#define 				SPI_Mode_3				 0


#define         SPI_CS_Pin		 GPIO_Pin_12
#define         SPI_CS_GPIO_Port    GPIOB


#define         SPI_CLK_Pin				 GPIO_Pin_13
#define         SPI_CLK_GPIO_Port       GPIOB

#define         SPI_MOSI_Pin			 GPIO_Pin_15
#define         SPI_MOSI_GPIO_Port      GPIOB

#define         SPI_MISO_Pin			 GPIO_Pin_9
#define         SPI_MISO_GPIO_Port      GPIOB



#define         SPI_CS_0()         GPIO_WriteBit(SPI_CS_GPIO_Port,SPI_CS_Pin,Bit_RESET)
#define         SPI_CS_1()         GPIO_WriteBit(SPI_CS_GPIO_Port,SPI_CS_Pin,Bit_SET)


#define         SPI_SCK_0()       GPIO_WriteBit(SPI_CLK_GPIO_Port,SPI_CLK_Pin,Bit_RESET)
#define         SPI_SCK_1()       GPIO_WriteBit(SPI_CLK_GPIO_Port,SPI_CLK_Pin,Bit_SET)

#define         SPI_MOSI_0()		  GPIO_WriteBit(SPI_MOSI_GPIO_Port,SPI_MOSI_Pin,Bit_RESET)
#define         SPI_MOSI_1()		  GPIO_WriteBit(SPI_MOSI_GPIO_Port,SPI_MOSI_Pin,Bit_SET)

#define   			SPI_MISO()				GPIO_ReadInputDataBit(SPI_MISO_GPIO_Port,SPI_MISO_Pin)

void Software_SPI_Init(void);
uint8_t Software_SPI_Write_Read(uint8_t data);
#endif


