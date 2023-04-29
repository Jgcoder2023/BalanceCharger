#ifndef __ADC_H__
#define __ADC_H__	 


#include "air32f10x.h"
#include "air32f10x_gpio.h"
#include "air32f10x_adc.h"



#define VREF (3330)

extern char VA[2][20];

void DMA_Configuration(void);
void ADC_Configuration(void);

#endif





