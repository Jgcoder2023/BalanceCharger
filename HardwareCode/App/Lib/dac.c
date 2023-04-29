#include "dac.h"


void DAC_Configuration(void)
{
	DAC_InitTypeDef	DAC_InitStructure;

	
	//��ʼ��DAC�����io��
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��GPIOAʱ��
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;//PA4,PA5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ��ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA4,GPIOA5
	
	

	//��ʼ��DAC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);//ʹ��DACʱ��
	
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;//�������
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;// DAC��������ģʽ:��
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;//���ǲ���ֵ
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;//�������ʹ��
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);//��ʼ��DACͨ��1
//	DAC_Init(DAC_Channel_2, &DAC_InitStructure);//��ʼ��DACͨ��2
	
	DAC_Cmd(DAC_Channel_1, ENABLE);//ʹ��DACͨ��1
//	DAC_Cmd(DAC_Channel_2, ENABLE);//ʹ��DACͨ��2
	
//	DAC_SetChannel1Data(DAC_Align_12b_L, 4095);//����DACͨ��1����
//	DAC_SetChannel2Data(DAC_Align_12b_R, 1);//����DACͨ��2����
}



//����Dac��ѹֵ 0 - 4095
//DAC��� = VREF x (DOR / 4095)
void DAC_SetValue(uint16_t DAC_DATA){
//	DAC_SetChannel2Data(DAC_Align_12b_R, DAC_DATA);//����DACͨ��2����
	DAC_SetChannel1Data(DAC_Align_12b_R, DAC_DATA);//����DACͨ��1����
	DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);//ʹ��DAC1�������
}



