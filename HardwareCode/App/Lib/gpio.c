
#include "gpio.h"
#define CN2 GPIO_Pin_4
#define CN3 GPIO_Pin_3
#define CN4 GPIO_Pin_15



//GPIO��ʼ��
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��GPIOBʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|CN2|CN3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //���ģʽ
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��GPIOB.2,10,11
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ��
	GPIO_InitStructure.GPIO_Pin = CN4;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��GPIOA15
	
	
	//��ʼ�����ƽ�����ص�io��
	GPIO_WriteBit(GPIOB,CN2,Bit_RESET);
	GPIO_WriteBit(GPIOB,CN3,Bit_RESET);
	GPIO_WriteBit(GPIOA,CN4,Bit_RESET);
}


void battery_two(uint8_t value){
	battery_close();
	if(value == 1){
		GPIO_WriteBit(GPIOB,CN2,Bit_SET);
	}
}

void battery_three(uint8_t value){
	battery_close();
	if(value == 1){
		GPIO_WriteBit(GPIOB,CN3,Bit_SET);
	}
}

void battery_four(uint8_t value){
	battery_close();
	if(value == 1){
		GPIO_WriteBit(GPIOA,CN4,Bit_SET);
	}
}

void battery_close(void){
	GPIO_WriteBit(GPIOB,CN2,Bit_RESET);
	GPIO_WriteBit(GPIOB,CN3,Bit_RESET);
	GPIO_WriteBit(GPIOA,CN4,Bit_RESET);
}



//ѡ����ʵĵ�ؿ���
void battery_select(uint8_t key){
	switch(key){
		case 9:
			battery_two(1);
			break;
		case 14:
			battery_three(1);
			break;
		case 19:
			battery_four(1);
			break;
		default:
			battery_close();
			break;
	}
}





//��תio״̬  ʹLED��˸
void GPIO_Turn(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	if(GPIO_ReadOutputDataBit(GPIOx,GPIO_Pin) == Bit_SET){
		GPIO_WriteBit(GPIOx,GPIO_Pin,Bit_RESET);
	}else{
		GPIO_WriteBit(GPIOx,GPIO_Pin,Bit_SET);
	}
	
}



