#ifndef __APP_H__
#define __APP_H__

#include "air32f10x.h"
#include "air32f10x_rcc.h"



//主菜单当前的状态
typedef enum{
	VoidMenu,//空菜单
	MaxMenu, //开始菜单
	SelectInVoltage, //选中输入电压
	SelectOutVoltage, //选中输出电压
	SetInVoltage, //设置输入电压
	SetOutVoltage,  //设置输出电压
	ExitSet, //退出设置
	SetOk //设置成功动画
}menu_state;

//设置输入输出状态
typedef enum{
	Input,
	Output
}setVoltage;


//定义输出电压与DAC的对应
//定义输入电压与值的对应
//显示值与实际值对应
typedef struct{
	uint16_t Value;
	char* voltage;
}show_value;


//定义一个记录输入与输出电压的key
typedef struct{
	uint8_t InKey;
	uint8_t OutKey;
}in_out_key;


extern in_out_key InOutKey;



void App_init(void);
void _0ms_func(void);
void _1ms_func(void);
void _10ms_func(void);
void _100ms_func(void);
void _1s_func(void);


void Menu_Init(void);
void SelectIn(void);	
void SelectOut(void);
void SetIn(uint8_t is_force_update);
void SetOut(uint8_t is_force_update);
void ExitSetMenu(void);
void SetOkMenu(void);
void SetVA(void);

#endif

