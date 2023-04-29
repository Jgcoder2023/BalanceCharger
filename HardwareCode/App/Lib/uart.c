#include "uart.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "air32f10x_rcc.h"
#include "esp8266.h"


USART_TypeDef *USART_3 = USART3;
USART_TypeDef *USART_1 = USART1;

//USART3 ���ڳ�ʼ��
void UART3_Configuration(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART_3, &USART_InitStructure);
	USART_Cmd(USART_3, ENABLE);
}


uint8_t DMA_Rec_Len = 255;   //���ջ������ĳ���
char Uart1_DMA_Rece_Buf[256];   //���ջ�����
//UART1��ʼ��
void UART1_Configuration(uint32_t bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE); //ʹ��          USART1��GPIOAʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��DMA����
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
 
    USART_DeInit(USART1);  //��λ����1
    //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
 
    //USART1_RX  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10
 
    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
 
    //USART ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
 
    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//���������ж�
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);   //ʹ�ܴ���1 DMA����
    USART_Cmd(USART1, ENABLE);                   //ʹ�ܴ��� 
 
    //��Ӧ��DMA����
    DMA_DeInit(DMA1_Channel5);   //��DMA��ͨ��5�Ĵ�������Ϊȱʡֵ  ����1��Ӧ����DMAͨ��5
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR; //DMA����usart����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Uart1_DMA_Rece_Buf;  //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
    DMA_InitStructure.DMA_BufferSize = DMA_Rec_Len;  //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
 
    DMA_Cmd(DMA1_Channel5, ENABLE);  //��ʽ����DMA����
}


//���»ָ�DMAָ��
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
    DMA_Cmd(DMA_CHx, DISABLE );  //�ر�USART1 TX DMA1��ָʾ��ͨ��    
    DMA_SetCurrDataCounter(DMA_CHx,DMA_Rec_Len);//DMAͨ����DMA����Ĵ�С
    DMA_Cmd(DMA_CHx, ENABLE);  //��USART1 TX DMA1��ָʾ��ͨ��  
}   



//Usart1�Ĵ����ж�
void USART1_IRQHandler( void )
{   
//    if(USART_GetITStatus( USART1, USART_IT_RXNE ) != RESET )    //���ֽ��ж�
//    {
//        Uart1ResData[i]  = (uint8_t)USART_ReceiveData( USART1 );
//        i++;         
//    }

//    if( USART_GetITStatus( USART1, USART_IT_IDLE ) == SET )                           //������߿���
//    {
//        USART_ReceiveData( USART1 );                                           //�������������жϱ�־λ���ȶ�USART_SR,Ȼ���USART_DR��
//    }  
	
	
	uint8_t Usart1_Rec_Cnt;  //��֡���ݳ���
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
      {
          USART_ReceiveData(USART1);//��ȡ����ע�⣺������Ҫ�������ܹ�����жϱ�־λ��
          Usart1_Rec_Cnt = DMA_Rec_Len-DMA_GetCurrDataCounter(DMA1_Channel5); //����ӱ�֡���ݳ���
 
         //***********֡���ݴ�����************//
//          printf ("Thelenght:%d\r\n",Usart1_Rec_Cnt);
        //*************************************//
         USART_ClearITPendingBit(USART1,USART_IT_IDLE);         //����жϱ�־
         MYDMA_Enable(DMA1_Channel5);                  //�ָ�DMAָ�룬�ȴ���һ�εĽ���
		  
		  
		  //����ص�����
		  uart_call_func(Usart1_Rec_Cnt,Uart1_DMA_Rece_Buf);
		  
		  
     } 

}



//����ʽ�����ַ���
void USART_Send(USART_TypeDef* USARTx,char * s){
	 for ( ; *s; s++) {
		USART_SendData(USARTx,*s);
        while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
	 }
}




//��ӡ��Ϣ
int SER_PutChar(int ch)
{
	while (!USART_GetFlagStatus(USART_3, USART_FLAG_TC));
	USART_SendData(USART_3, (uint8_t)ch);

	return ch;
}

int fputc(int c, FILE *f)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	if (c == '\n')
	{
		SER_PutChar('\r');
	}
	return (SER_PutChar(c));
}

