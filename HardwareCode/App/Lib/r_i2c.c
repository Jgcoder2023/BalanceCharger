#include "r_i2c.h"
#include "delay.h"



/**
 * 初始化i2c的IO
 * 这里定义了两路i2c  实际一路即可
*/
void I2C_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* PROBE_SCL时钟线配置-推挽 */
	GPIO_InitStructure.GPIO_Pin   = PROBE_SCL_Pin;
	GPIO_Init(PROBE_SCL_Port, &GPIO_InitStructure);
  
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;

	
	/* PROBE_SDA数据线配置-开漏 */
	GPIO_InitStructure.GPIO_Pin   = PROBE_SDA_Pin;
	GPIO_Init(PROBE_SDA_Port, &GPIO_InitStructure);
//	GPIO_ForcePullUpConfig(GPIOB, PROBE_SDA_Pin); // PB7上拉
	
	PROBE_SCL_SET;
	PROBE_SDA_SET;
}


void SDA_OUT(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = PROBE_SDA_Pin;
	GPIO_Init(PROBE_SDA_Port, &GPIO_InitStructure);
}

void SDA_IN(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = PROBE_SDA_Pin;
	GPIO_Init(PROBE_SDA_Port, &GPIO_InitStructure);
}





/*
*********************************************************************************************************
*	函 数 名: I2C_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void I2C_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
		PROBE_SDA_SET;
		PROBE_SCL_SET;
		Delay_Us(AT24MACxx_CLK_us);
		PROBE_SDA_RESET;
		Delay_Us(AT24MACxx_CLK_us);
		PROBE_SCL_RESET;
		Delay_Us(AT24MACxx_CLK_us);
}

/*
*********************************************************************************************************
*	函 数 名: I2C_Stop
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void I2C_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
		  Delay_Us(AT24MACxx_CLK_us);
			PROBE_SDA_RESET;
			Delay_Us(AT24MACxx_CLK_us);
			PROBE_SCL_SET;
			Delay_Us(AT24MACxx_CLK_us);
			PROBE_SDA_SET;
}

/*
*********************************************************************************************************
*	函 数 名: I2C_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void I2C_SendByte(uint8_t _ucByte)
{
	uint8_t i;

		  for (i = 0; i < 8; i++)
			{		
				if (_ucByte & 0x80)/* 先发送字节的高位bit7 */
				{
			    PROBE_SDA_SET;
				}
				else
				{
				  PROBE_SDA_RESET;
				}
				Delay_Us(AT24MACxx_CLK_us);
				PROBE_SCL_SET;
				Delay_Us(AT24MACxx_CLK_us);
				PROBE_SCL_RESET;
				_ucByte <<= 1;	/* 左移一个bit */
				
				if (i == 7)
				{
				  PROBE_SDA_SET; // 释放总线
				}
			}
}

/*
*********************************************************************************************************
*	函 数 名: I2C_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t I2C_ReadByte(void)
{
	uint8_t i;
	uint8_t value;
	
			PROBE_SDA_SET;//释放SDA线控制权
			/* 读到第1个bit为数据的bit7 */
			value = 0;
			for (i = 0; i < 8; i++)
			{
				value <<= 1;
				PROBE_SCL_SET;
				Delay_Us(AT24MACxx_CLK_us);
				if (Read_PROBE_SDA)
				{
					value++;
				}
				PROBE_SCL_RESET;//下降沿从器件更新SDA状态
				Delay_Us(AT24MACxx_CLK_us);
			}
	
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: I2C_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t I2C_WaitAck(void)
{
	uint8_t ret;

			PROBE_SDA_SET;	/* CPU释放SDA总线 */
			Delay_Us(AT24MACxx_CLK_us);
			PROBE_SCL_SET;	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
			
			Delay_Us(AT24MACxx_CLK_us);
			if (Read_PROBE_SDA)	/* CPU读取SDA口线状态 */
			{
				ret = 1;
			}
			else
			{
				ret = 0;
			}
			
			PROBE_SCL_RESET;//下降沿从器件会释放总线
			Delay_Us(AT24MACxx_CLK_us);
	return ret;
}

/*
*********************************************************************************************************
*	函 数 名: I2C_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void I2C_Ack(void)
{
			PROBE_SDA_RESET;	/* CPU驱动SDA = 0 */
			Delay_Us(AT24MACxx_CLK_us);
			PROBE_SCL_SET;	  /* CPU产生1个时钟 */
			Delay_Us(AT24MACxx_CLK_us);
			PROBE_SCL_RESET;
			PROBE_SDA_SET;	  /* CPU释放SDA总线 */
			Delay_Us(AT24MACxx_CLK_us);/* 必须要延迟一会儿等待从机操作数据总线 */
}

/*
*********************************************************************************************************
*	函 数 名: I2C_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void I2C_NAck(void)
{
			PROBE_SDA_SET;	/* CPU驱动SDA = 1 */
			Delay_Us(AT24MACxx_CLK_us);
			PROBE_SCL_SET;	/* CPU产生1个时钟 */
			Delay_Us(AT24MACxx_CLK_us);
			PROBE_SCL_RESET;
			Delay_Us(AT24MACxx_CLK_us);
}



 
//oled发送数据
uint8_t OLED_Data(uint8_t dev_addr,uint8_t address,uint8_t data)
{
   I2C_Start();
   I2C_SendByte(dev_addr);			//D/C#=0; R/W#=0
	 if(I2C_WaitAck())
	 {
		 I2C_Stop();		 
		 return 1;		
	 }
   I2C_SendByte(address);			//write data
	 if(I2C_WaitAck())
	 {
		 I2C_Stop();		 
		 return 1;		
	 }
   I2C_SendByte(data);
	 if(I2C_WaitAck())
	 {
		 I2C_Stop();		 
		 return 1;		
	 }
   I2C_Stop();
	 return 0;
}
