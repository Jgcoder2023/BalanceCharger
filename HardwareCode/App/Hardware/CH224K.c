#include "CH224K.h"



//Ch224k 初始化  pa5   pa6   pa7

void Ch224k_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能GPIOB时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //输出模式
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	CH224K_XV(5);   //先设置最低电压
}


 
/**
 * CFG1  CFG2  CFG3
 * 1      -     -     5V
 * 0      0     0     9V
 * 0      0     1     12V
 * 0      1     1     15V
 * 0      1     0     20V
 */
//设置输出的电压
void CH224K_XV(uint8_t value){
	switch(value){
		case 9:
			GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
			break;
		case 12:
			GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
			break;
		case 15:
			GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
			break;
		case 20:
			GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
			break;
		default:
			GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
			break;
	}
}

