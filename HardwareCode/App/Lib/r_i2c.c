#include "r_i2c.h"
#include "delay.h"



/**
 * ��ʼ��i2c��IO
 * ���ﶨ������·i2c  ʵ��һ·����
*/
void I2C_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* PROBE_SCLʱ��������-���� */
	GPIO_InitStructure.GPIO_Pin   = PROBE_SCL_Pin;
	GPIO_Init(PROBE_SCL_Port, &GPIO_InitStructure);
  
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;

	
	/* PROBE_SDA����������-��© */
	GPIO_InitStructure.GPIO_Pin   = PROBE_SDA_Pin;
	GPIO_Init(PROBE_SDA_Port, &GPIO_InitStructure);
//	GPIO_ForcePullUpConfig(GPIOB, PROBE_SDA_Pin); // PB7����
	
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
*	�� �� ��: I2C_Start
*	����˵��: CPU����I2C���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void I2C_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
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
*	�� �� ��: I2C_Stop
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void I2C_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
		  Delay_Us(AT24MACxx_CLK_us);
			PROBE_SDA_RESET;
			Delay_Us(AT24MACxx_CLK_us);
			PROBE_SCL_SET;
			Delay_Us(AT24MACxx_CLK_us);
			PROBE_SDA_SET;
}

/*
*********************************************************************************************************
*	�� �� ��: I2C_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void I2C_SendByte(uint8_t _ucByte)
{
	uint8_t i;

		  for (i = 0; i < 8; i++)
			{		
				if (_ucByte & 0x80)/* �ȷ����ֽڵĸ�λbit7 */
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
				_ucByte <<= 1;	/* ����һ��bit */
				
				if (i == 7)
				{
				  PROBE_SDA_SET; // �ͷ�����
				}
			}
}

/*
*********************************************************************************************************
*	�� �� ��: I2C_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t I2C_ReadByte(void)
{
	uint8_t i;
	uint8_t value;
	
			PROBE_SDA_SET;//�ͷ�SDA�߿���Ȩ
			/* ������1��bitΪ���ݵ�bit7 */
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
				PROBE_SCL_RESET;//�½��ش���������SDA״̬
				Delay_Us(AT24MACxx_CLK_us);
			}
	
	return value;
}

/*
*********************************************************************************************************
*	�� �� ��: I2C_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t I2C_WaitAck(void)
{
	uint8_t ret;

			PROBE_SDA_SET;	/* CPU�ͷ�SDA���� */
			Delay_Us(AT24MACxx_CLK_us);
			PROBE_SCL_SET;	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
			
			Delay_Us(AT24MACxx_CLK_us);
			if (Read_PROBE_SDA)	/* CPU��ȡSDA����״̬ */
			{
				ret = 1;
			}
			else
			{
				ret = 0;
			}
			
			PROBE_SCL_RESET;//�½��ش��������ͷ�����
			Delay_Us(AT24MACxx_CLK_us);
	return ret;
}

/*
*********************************************************************************************************
*	�� �� ��: I2C_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void I2C_Ack(void)
{
			PROBE_SDA_RESET;	/* CPU����SDA = 0 */
			Delay_Us(AT24MACxx_CLK_us);
			PROBE_SCL_SET;	  /* CPU����1��ʱ�� */
			Delay_Us(AT24MACxx_CLK_us);
			PROBE_SCL_RESET;
			PROBE_SDA_SET;	  /* CPU�ͷ�SDA���� */
			Delay_Us(AT24MACxx_CLK_us);/* ����Ҫ�ӳ�һ����ȴ��ӻ������������� */
}

/*
*********************************************************************************************************
*	�� �� ��: I2C_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void I2C_NAck(void)
{
			PROBE_SDA_SET;	/* CPU����SDA = 1 */
			Delay_Us(AT24MACxx_CLK_us);
			PROBE_SCL_SET;	/* CPU����1��ʱ�� */
			Delay_Us(AT24MACxx_CLK_us);
			PROBE_SCL_RESET;
			Delay_Us(AT24MACxx_CLK_us);
}



 
//oled��������
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
