#include "dac.h"


void DAC_Configuration(void)
{
	DAC_InitTypeDef	DAC_InitStructure;

	
	//初始化DAC输出的io口
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOA时钟
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;//PA4,PA5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA4,GPIOA5
	
	

	//初始化DAC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);//使能DAC时钟
	
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;//软件触发
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;// DAC波形生成模式:无
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;//三角波幅值
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;//输出缓冲使能
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);//初始化DAC通道1
//	DAC_Init(DAC_Channel_2, &DAC_InitStructure);//初始化DAC通道2
	
	DAC_Cmd(DAC_Channel_1, ENABLE);//使能DAC通道1
//	DAC_Cmd(DAC_Channel_2, ENABLE);//使能DAC通道2
	
//	DAC_SetChannel1Data(DAC_Align_12b_L, 4095);//设置DAC通道1数据
//	DAC_SetChannel2Data(DAC_Align_12b_R, 1);//设置DAC通道2数据
}



//设置Dac电压值 0 - 4095
//DAC输出 = VREF x (DOR / 4095)
void DAC_SetValue(uint16_t DAC_DATA){
//	DAC_SetChannel2Data(DAC_Align_12b_R, DAC_DATA);//设置DAC通道2数据
	DAC_SetChannel1Data(DAC_Align_12b_R, DAC_DATA);//设置DAC通道1数据
	DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);//使能DAC1软件触发
}



