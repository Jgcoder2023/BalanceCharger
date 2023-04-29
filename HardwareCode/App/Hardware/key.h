#ifndef __KEY_H__
#define __KEY_H__

#include "air32f10x.h"
#include "air32f10x_gpio.h"


#define MaxButtonNumber              3//按键数量,如需32,更改u16Bit_Key_struct位域为32位

#define PROBE1_KEY_Port              GPIOB
#define PROBE1_KEY_Pin               GPIO_Pin_0
#define Read_PROBE1_KEY              GPIO_ReadInputDataBit(PROBE1_KEY_Port, PROBE1_KEY_Pin)

#define PROBE2_KEY_Port              GPIOB
#define PROBE2_KEY_Pin               GPIO_Pin_1
#define Read_PROBE2_KEY              GPIO_ReadInputDataBit(PROBE2_KEY_Port, PROBE2_KEY_Pin)

#define PROBE3_KEY_Port              GPIOB
#define PROBE3_KEY_Pin               GPIO_Pin_2
#define Read_PROBE3_KEY              GPIO_ReadInputDataBit(PROBE3_KEY_Port, PROBE3_KEY_Pin)


//**********************************************************
struct u32_Bit_Key_struct
{
	u32 Bit0 : 1;
	u32 Bit1 : 1;
	u32 Bit2 : 1;
	u32 Bit3 : 1;
	u32 Bit4 : 1;
	u32 Bit5 : 1;
	u32 Bit6 : 1;
	u32 Bit7 : 1;
	u32 Bit8 : 1;
	u32 Bit9 : 1;
	u32 Bit10: 1;
	u32 Bit11: 1;
	u32 Bit12: 1;
	u32 Bit13: 1;
	u32 Bit14: 1;
	u32 Bit15: 1;
	u32 Bit16: 1;
	u32 Bit17: 1;
	u32 Bit18: 1;
	u32 Bit19: 1;
	u32 Bit20: 1;
	u32 Bit21: 1;
	u32 Bit22: 1;
	u32 Bit23: 1;
	u32 Bit24: 1;
	u32 Bit25: 1;
	u32 Bit26: 1;
	u32 Bit27: 1;
	u32 Bit28: 1;
	u32 Bit29: 1;
	u32 Bit30: 1;
	u32 Bit31: 1;
};
//*************
union u32_Bit_Key_union
{
	struct u32_Bit_Key_struct u32_Bit;
	u32 All;
};

#define  PROBE1_KEY      0//按键1
#define  PROBE2_KEY      1//按键2
#define  PROBE3_KEY      2//按键3


void KeyIO_Init(void);       //键IO初始化
void KeyInputScan(void);     //开关量扫描输入
void KeyValueCalculate(void);//键值计算
uint8_t KeyState(uint8_t KeyNumber);// 返回键值的当前状态
uint8_t R_KeyState(uint8_t KeyNumber);//返回键值的上升沿状态
uint8_t F_KeyState(uint8_t KeyNumber);//返回键值的下降沿状态
uint8_t L_KeyState(uint8_t KeyNumber);//返回键值的长按状态
uint8_t D_KeyState(uint8_t KeyNumber);//返回键值的断续状态
uint8_t DC_KeyState(uint8_t KeyNumber);//双击之后触发一次
#endif


/**
 * 
 * 使用指南
 * 1. 先调用 KeyIO_Init 进行io口初始化
 * 2. 每10ms 调用 KeyInputScan 进行按键扫描
 * 3. 每10ms 调用 KeyValueCalculate 进行按键状态处理
 * 4. 调用对应函数 返回对应按键对应的状态
 * 
*/




