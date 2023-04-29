#include "uart.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "air32f10x_rcc.h"
#include "esp8266.h"


USART_TypeDef *USART_3 = USART3;
USART_TypeDef *USART_1 = USART1;

//USART3 串口初始化
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


uint8_t DMA_Rec_Len = 255;   //接收缓冲区的长度
char Uart1_DMA_Rece_Buf[256];   //接收缓冲区
//UART1初始化
void UART1_Configuration(uint32_t bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE); //使能          USART1，GPIOA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA传输
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
 
    USART_DeInit(USART1);  //复位串口1
    //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
 
    //USART1_RX  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10
 
    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
 
    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
 
    USART_Init(USART1, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//开启空闲中断
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);   //使能串口1 DMA接收
    USART_Cmd(USART1, ENABLE);                   //使能串口 
 
    //相应的DMA配置
    DMA_DeInit(DMA1_Channel5);   //将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR; //DMA外设usart基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Uart1_DMA_Rece_Buf;  //DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
    DMA_InitStructure.DMA_BufferSize = DMA_Rec_Len;  //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //数据宽度为8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道
 
    DMA_Cmd(DMA1_Channel5, ENABLE);  //正式驱动DMA传输
}


//重新恢复DMA指针
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
    DMA_Cmd(DMA_CHx, DISABLE );  //关闭USART1 TX DMA1所指示的通道    
    DMA_SetCurrDataCounter(DMA_CHx,DMA_Rec_Len);//DMA通道的DMA缓存的大小
    DMA_Cmd(DMA_CHx, ENABLE);  //打开USART1 TX DMA1所指示的通道  
}   



//Usart1的串口中断
void USART1_IRQHandler( void )
{   
//    if(USART_GetITStatus( USART1, USART_IT_RXNE ) != RESET )    //单字节中断
//    {
//        Uart1ResData[i]  = (uint8_t)USART_ReceiveData( USART1 );
//        i++;         
//    }

//    if( USART_GetITStatus( USART1, USART_IT_IDLE ) == SET )                           //如果总线空闲
//    {
//        USART_ReceiveData( USART1 );                                           //由软件序列清除中断标志位（先读USART_SR,然后读USART_DR）
//    }  
	
	
	uint8_t Usart1_Rec_Cnt;  //本帧数据长度
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) //接收中断(接收到的数据必须是0x0d 0x0a结尾)
      {
          USART_ReceiveData(USART1);//读取数据注意：这句必须要，否则不能够清除中断标志位。
          Usart1_Rec_Cnt = DMA_Rec_Len-DMA_GetCurrDataCounter(DMA1_Channel5); //算出接本帧数据长度
 
         //***********帧数据处理函数************//
//          printf ("Thelenght:%d\r\n",Usart1_Rec_Cnt);
        //*************************************//
         USART_ClearITPendingBit(USART1,USART_IT_IDLE);         //清除中断标志
         MYDMA_Enable(DMA1_Channel5);                  //恢复DMA指针，等待下一次的接收
		  
		  
		  //处理回调函数
		  uart_call_func(Usart1_Rec_Cnt,Uart1_DMA_Rece_Buf);
		  
		  
     } 

}



//阻塞式发送字符串
void USART_Send(USART_TypeDef* USARTx,char * s){
	 for ( ; *s; s++) {
		USART_SendData(USARTx,*s);
        while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
	 }
}




//打印信息
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

