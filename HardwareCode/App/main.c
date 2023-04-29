#include "app.h"
#include "stdint.h"
#include "time.h"

uint8_t _1ms_flag,_10ms_flag,_100ms_flag,_1s_flag;



void App_Loop(void);


int main(void)
{
	App_init();
    while (1){
        App_Loop();
    }
}


void SystemInit(){}



//循环体
void App_Loop(void) {
	_0ms_func();
    if(_1ms_flag){
        _1ms_flag=0;
        _1ms_func();
    }

    if(_10ms_flag){
        _10ms_flag=0;
        _10ms_func();
    }
    if(_100ms_flag){
        _100ms_flag=0;
        _100ms_func();
    }
    if(_1s_flag){
        _1s_flag=0;
        _1s_func();
    }

}



//定时器3中断服务程序(1ms中断回调)
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		static uint8_t _10ms,_100ms,_1s;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
		
		_1ms_flag=1;
		_10ms++;
		if(_10ms >= 10){
			_10ms = 0;
			_100ms++;
			_10ms_flag=1;
		}
		if(_100ms >= 10){
			_100ms = 0;
			_1s++;
			_100ms_flag=1;
		}
		if(_1s >= 10){
			_1s=0;
			_1s_flag=1;
		}
		
	}
}



