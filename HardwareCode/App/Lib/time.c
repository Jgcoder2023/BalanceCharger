#include "time.h"
#include "uart.h"


RCC_ClocksTypeDef clocks; 


//��ʱ��TIM3��ʼ��
void TIM_Configuration(void)
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	RCC_GetClocksFreq(&clocks);//��ȡϵͳʱ��Ƶ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 215; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = 999; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	//TIM_TimeBaseStructure.TIM_Prescaler = clocks.PCLK1_Frequency/1000000 - 1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx	
}


////��ʱ��3�жϷ������
//void TIM3_IRQHandler(void)   //TIM3�ж�
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
//	{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־ 
////		printf("%s\n",__FUNCTION__);
//	}
//}


//��ӡƵ����Ϣ
void log_clocks(void){
	printf("\n");
	printf("SYSCLK: %3.1fMhz, HCLK: %3.1fMhz, APB1: %3.1fMhz, APB2: %3.1fMhz, ADCCLK: %3.1fMhz\n",
			   (float)clocks.SYSCLK_Frequency / 1000000, (float)clocks.HCLK_Frequency / 1000000,
			   (float)clocks.PCLK1_Frequency / 1000000, (float)clocks.PCLK2_Frequency / 1000000, 
				(float)clocks.ADCCLK_Frequency / 1000000);
}

