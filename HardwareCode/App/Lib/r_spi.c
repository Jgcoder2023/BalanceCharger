#include "r_spi.h"
#include "delay.h"

void Software_SPI_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能GPIOB时钟

	GPIO_InitStructure.GPIO_Pin = SPI_CLK_Pin|SPI_MOSI_Pin|SPI_CS_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //输出模式
	GPIO_Init(SPI_CS_GPIO_Port, &GPIO_InitStructure); //初始化GPIOB


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
	GPIO_Init(SPI_MISO_GPIO_Port,&GPIO_InitStructure);
	
	SPI_CS_1();
	SPI_SCK_1();
	SPI_MOSI_1();
	
}

#if SPI_Mode_0
/*
Mode_0
MSB先行
data :待写入的数据
*/
uint8_t Software_SPI_Write_Read(uint8_t data)
{
	uint8_t i;
	uint8_t redata;
	
	for(i=0;i<8;i++)
	{
		SPI_SCK_0();
		Delay_Us(10);
		if(data & 0x80)
		{
			SPI_MOSI_1();
		}
		else
		{
			SPI_MOSI_0();
		}
		data <<= 1;
		SPI_SCK_1();
		Delay_Us(10);		
		redata<<=1;
		if(SPI_MISO())
		{
			redata++;
		}
	}

	return redata;
}



#elif SPI_Mode_1
/*

Mode_1
MSB先行
data :待写入的数据
*/
u8 Software_SPI_Write_Read(u8 data)
{
	u8 i;
	u8 redata;
	for(i=0;i<8;i++)
	{
		SPI_SCK_1();
		HAL_Delay_us(10);
		if(data & 0x80)
		{
			SPI_MOSI_1();
		}
		else
		{
			SPI_MOSI_0();
		}
		data <<= 1;
		SPI_SCK_0();
		HAL_Delay_us(10);		
		redata<<=1;
		if(SPI_MISO())
		{
			redata++;
		}
	}
	
	return redata;
}

#elif SPI_Mode_2
/*

Mode_2
MSB先行
data :待写入的数据
*/
u8 Software_SPI_Write_Read(u8 data)
{
	u8 i;
	u8 redata;
	for(i=0;i<8;i++)
	{
		SPI_SCK_1();
		HAL_Delay_us(10);
		if(data & 0x80)
		{
			SPI_MOSI_1();
		}
		else
		{
			SPI_MOSI_0();
		}
		data <<= 1;
		SPI_SCK_0();
		HAL_Delay_us(10);		
		redata<<=1;
		if(SPI_MISO())
		{
			redata++;
		}
	}
	
	return redata;
}
#elif SPI_Mode_3
/*

Mode_3
MSB先行
data :待写入的数据
*/

u8 Software_SPI_Write_Read(u8 data)
{
	u8 i;
	u8 redata;
	SPI_SCK_1();
	HAL_Delay_us(10);
	for(i=0;i<8;i++)
	{
		SPI_SCK_0();
		HAL_Delay_us(10);	
		if(data & 0x80)
		{
			SPI_MOSI_1();
		}
		else
		{
			SPI_MOSI_0();
		}
		data <<= 1;
		SPI_SCK_1();
		HAL_Delay_us(10);	
		redata<<=1;
		if(SPI_MISO())
		{
			redata++;
		}
	}
	return redata;
}
#endif

