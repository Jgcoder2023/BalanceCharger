
#ifndef __UART_H__
#define __UART_H__

/* Includes ------------------------------------------------------------------*/
#include "air32f10x.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "air32f10x_usart.h"




extern char Uart1_DMA_Rece_Buf[256];   //Ω” ’ª∫≥Â«¯


void UART3_Configuration(uint32_t bound);
int SER_PutChar(int ch);

void UART1_Configuration(uint32_t bound);
void USART_Send(USART_TypeDef* USARTx,char * s);
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);

#endif


