#include "rcc.h"



//����ʱ����
void RCC_ClkConfiguration(void)
{
	RCC_DeInit(); //��λRCC�Ĵ���

	RCC_HSEConfig(RCC_HSE_ON); //ʹ��HSE
	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
		; //�ȴ�HSE����

	RCC_PLLCmd(DISABLE);										 //�ر�PLL
	AIR_RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_27, FLASH_Div_2); //����PLL,8*27=216MHz

	RCC_PLLCmd(ENABLE); //ʹ��PLL
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //�ȴ�PLL����

	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //ѡ��PLL��Ϊϵͳʱ��

	RCC_HCLKConfig(RCC_SYSCLK_Div1); //����AHBʱ��
	RCC_PCLK1Config(RCC_HCLK_Div2);	 //����APB1ʱ��
	RCC_PCLK2Config(RCC_HCLK_Div1);	 //����APB2ʱ��

	RCC_LSICmd(ENABLE); //ʹ���ڲ�����ʱ��
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);//�ȴ�LSI����
	RCC_HSICmd(ENABLE); //ʹ���ڲ�����ʱ��
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); //�ȴ�HSI����
}


