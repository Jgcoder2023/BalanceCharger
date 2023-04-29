#include "app.h"
#include "uart.h"
#include "gpio.h"
#include "time.h"
#include "rcc.h"
#include "oled.h"
#include "CH224K.h"
#include "key.h"
#include "MCP41010T.h"
#include "r_spi.h"
#include "r_i2c.h"
#include "dac.h"
#include "adc.h"
#include "esp8266.h"


menu_state MenuState;  //菜单的定义
menu_state DisplayState;  //显示菜单的定义  防止屏幕持续刷新
in_out_key InOutKey;

int8_t MaxMenuKey = 0;

//输出电压数据的定义
show_value DacVolValue[24] = {
	{4095,":Min  "},{3880,":2V   "},{3800,":3V   "},{3720,":4V   "},{3705,":4.2V "},{3640,":5V   "},{3560,":6V   "},{3475,":7V   "},
	{3390,":8V   "},{3360,":8.4V "},{3350,":9V   "},{3220,":10V  "},{3135,":11V  "},{3055,":12V  "},{3000,":12.6V"},{2970,":13V  "},
	{2880,":14V  "},{2790,":15V  "},{2700,":16V  "},{2630,":16.8V"},{2610,":17V  "},{2520,":18V  "},{2420,":19V  "},{0,":Max  "}
};

//输入电压的定义
show_value InVolValue[5] = {
	{5,":5V "},{9,":9V "},{12,":12V"},{15,":15V"},{20,":20V"}
};


void App_init(void){
	RCC_ClkConfiguration();
//	GPIO_PinRemapConfig(GPIO_Remap_SWJJTAGDISABLE, ENABLE);
	Delay_Init();				//延时初始化
	TIM_Configuration();
	UART3_Configuration(115200); //默认为串口，波特率115200
	UART1_Configuration(115200);
	GPIO_Configuration();   //led的初始化
	
	InOutKey.InKey = 0;
	InOutKey.OutKey = 0;
	
	Delay_Ms(200);//等待模块
	
	I2C_IO_Init();//I2C初始化

	log_clocks();
	
	Ch224k_Init();

	KeyIO_Init();
	Software_SPI_Init();
	
	
	DAC_Configuration(); //dac初始化
	
	ADC_Configuration();//ADC初始化
	
  //oled的初始化
   OLED_Init();                           //OLED初始
   OLED_Clear();                         //清屏
	
	ESP8266Init();
	
	Delay_Ms(200);
	DisplayState = VoidMenu;
	MenuState = MaxMenu;
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 开启ADC1软件转换
	DAC_SetValue(4095);
}


void _0ms_func(void){}
	

void _1ms_func(void){
	
}
	

void _10ms_func(void){
	KeyInputScan();
	KeyValueCalculate();
	
	switch (MenuState){
		case MaxMenu: //开始菜单
			Menu_Init();
			if(L_KeyState(PROBE2_KEY)){
				//长按set   进入设置模式
				MenuState = SelectInVoltage;
				OLED_Clear();//清屏
			}
			
			if(R_KeyState(PROBE1_KEY)){ //set下降沿
				MaxMenuKey = -2;
				DisplayState = VoidMenu;
				return;
			}
			if(R_KeyState(PROBE3_KEY)){ //set下降沿
				MaxMenuKey = 0;
				DisplayState = VoidMenu;
				return;
			}
			
			
		break;
			
		case SelectInVoltage:   //选中输入电压
			SelectIn();
			if(R_KeyState(PROBE2_KEY)){ //set下降沿
				MenuState = SetInVoltage;
				return;
			}
			if(R_KeyState(PROBE1_KEY)){ //单击<
				MenuState = ExitSet;
				return;
			}
			if(R_KeyState(PROBE3_KEY)){ //单击>
				MenuState = SelectOutVoltage;
				return;
			}
		break;
		
		case SelectOutVoltage:  //选中输出电压
			SelectOut();
			if(R_KeyState(PROBE2_KEY)){ //set下降沿
				MenuState = SetOutVoltage;
				return;
			}
			if(R_KeyState(PROBE1_KEY)){ //单击<
				MenuState = SelectInVoltage;
				return;
			}
			if(R_KeyState(PROBE3_KEY)){ //单击>
				MenuState = ExitSet;
				return;
			}
			break;
			
		case ExitSet:  //退出设置
			ExitSetMenu();
			if(R_KeyState(PROBE2_KEY)){ //set下降沿
				MenuState = MaxMenu;
				return;
			}
			if(R_KeyState(PROBE1_KEY)){ //单击<
				MenuState = SelectOutVoltage;
				return;
			}
			if(R_KeyState(PROBE3_KEY)){ //单击>
				MenuState = SelectInVoltage;
				return;
			}
			break;
		
		case SetInVoltage:   //设置输入电压
			SetIn(0);
			if(L_KeyState(PROBE2_KEY)){   //长按set 返回
				MenuState = SelectInVoltage;
			}
			
			if(D_KeyState(PROBE1_KEY)){ //单击<
				if(InOutKey.InKey <= 0){
					InOutKey.InKey = 4;
					SetIn(1);
					return;
				}
				InOutKey.InKey--;
				SetIn(1);
				return;
			}
			if(D_KeyState(PROBE3_KEY)){ //单击>
				if(InOutKey.InKey >= 4){
					InOutKey.InKey = 0;
					SetIn(1);
					return;
				}
				InOutKey.InKey++;
				SetIn(1);
				return;
			}
			if(R_KeyState(PROBE2_KEY)){ //单击set
				MenuState = SetOk;
				SetOkMenu();
				//设置具体的电压
				CH224K_XV(InVolValue[InOutKey.InKey].Value);
				return;
			}
			break;
		
		case SetOutVoltage:    //设置输出电压
			SetOut(0);
			if(L_KeyState(PROBE2_KEY)){   //长按set 返回
				MenuState = SelectOutVoltage;
			}
			if(D_KeyState(PROBE1_KEY)){ //单击<
				if(InOutKey.OutKey <= 0){
					InOutKey.OutKey = 23;
					SetOut(1);
					return;
				}
				InOutKey.OutKey--;
				SetOut(1);
				return;
			}
			if(D_KeyState(PROBE3_KEY)){ //单击>
				if(InOutKey.OutKey >= 23){
					InOutKey.OutKey = 0;
					SetOut(1);
					return;
				}
				InOutKey.OutKey++;
				SetOut(1);
				return;
			}
			if(R_KeyState(PROBE2_KEY)){ //单击set
				MenuState = SetOk;
				SetOkMenu();
				DAC_SetValue(DacVolValue[InOutKey.OutKey].Value);
				battery_select(InOutKey.OutKey);
				return;
			}
			break;
		
		
		default:
		break;
	}
	
	
}

//uint16_t DACValue=0;
void _100ms_func(void){
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//	DAC_SetValue(DACValue);
}


void _1s_func(void){
	char str[100] = {0};
	GPIO_Turn(GPIOB,GPIO_Pin_12); //状态灯闪烁
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 开启ADC1软件转换
	ADC_SoftwareStartConvCmd(ADC2, ENABLE); // 开启ADC1软件转换
	
	
	if(MenuState == SetOk){
		//使其成功界面播放一秒
		SetOkMenu();
	}
	
	if(MenuState == SetInVoltage){
		SetIn(1);
	}
	if(MenuState == SetOutVoltage){
		SetOut(1);
	}
	
	if(MenuState == MaxMenu){
		OLED_ShowString(64,0,VA[0],16, 0);
		OLED_ShowString(64,2,VA[1],16, 0);
		OLED_ShowString(64,4,InVolValue[InOutKey.InKey].voltage,16, 0);
		OLED_ShowString(64,6,DacVolValue[InOutKey.OutKey].voltage,16, 0);
	}
	
	

	Esp8266_func();   //8266的联网初始化
	
	
	//发送数据给服务端
	//设置输入，设置输出，输出电压，输出电流
	sprintf(str,"%d,%d,%s,%s,#",InOutKey.InKey,InOutKey.OutKey,VA[0],VA[1]);
	ESP_SendMsg(str);

}



//初始化菜单
void Menu_Init(void){
	if(DisplayState == MaxMenu){
		return;
	}
	DisplayState = MaxMenu;


	OLED_ShowCHinese(0,0,0,0); //正相显示汉字“输”
	OLED_ShowCHinese(16,0,2,0);//正相显示汉字“出”
	OLED_ShowCHinese(32,0,3,0);//正相显示汉字“电”
	OLED_ShowCHinese(48,0,4,0);//正相显示汉字“压”
	OLED_ShowString(64,0,VA[0],16, 0);


	
	//输出电流
	OLED_ShowCHinese(0,2,0,0);
	OLED_ShowCHinese(16,2,2,0);
	OLED_ShowCHinese(32,2,3,0);
	OLED_ShowCHinese(48,2,5,0);
	OLED_ShowString(64,2,VA[1],16, 0);
	
	
	
	//设置输入
	OLED_ShowCHinese(0,4,6,0);
	OLED_ShowCHinese(16,4,7,0);
	OLED_ShowCHinese(32,4,0,0);
	OLED_ShowCHinese(48,4,1,0);
	OLED_ShowString(64,4,InVolValue[InOutKey.InKey].voltage,16, 0);
	
	
	//设置输出
	OLED_ShowCHinese(0,6,6,0);
	OLED_ShowCHinese(16,6,7,0);
	OLED_ShowCHinese(32,6,0,0);
	OLED_ShowCHinese(48,6,2,0);
	OLED_ShowString(64,6,DacVolValue[InOutKey.OutKey].voltage,16, 0);
}


//选中输入电压
void SelectIn(void){	
	
	if(DisplayState == SelectInVoltage){
		return;
	}
	DisplayState = SelectInVoltage;
	
	OLED_ShowCHinese(0,0,23,0);
	OLED_ShowCHinese(16,0,6,0); //正相显示汉字“设”
	OLED_ShowCHinese(32,0,7,0);//正相显示汉字“置”
	OLED_ShowCHinese(48,0,8,0);//正相显示汉字“模”
	OLED_ShowCHinese(64,0,9,0);//正相显示汉字“式”
	
	
	OLED_ShowCHinese(0,2,19,1);
	OLED_ShowCHinese(16,2,0,1); //正相显示汉字“输”
	OLED_ShowCHinese(32,2,1,1);//正相显示汉字“入”
	OLED_ShowString(48,2,InVolValue[InOutKey.InKey].voltage,16, 1);
	
	OLED_ShowCHinese(0,4,20,0);
	OLED_ShowCHinese(16,4,0,0); //正相显示汉字“输”
	OLED_ShowCHinese(32,4,2,0);//正相显示汉字“出”
	OLED_ShowString(48,4,DacVolValue[InOutKey.OutKey].voltage,16, 0);
	
	OLED_ShowCHinese(0,6,21,0);
	OLED_ShowCHinese(16,6,13,0); //正相显示汉字“退”
	OLED_ShowCHinese(32,6,14,0); //正相显示汉字“出”
	OLED_ShowCHinese(48,6,6,0); //正相显示汉字“设”
	OLED_ShowCHinese(64,6,7,0);//正相显示汉字“置”
}


//选中输出电压
void SelectOut(void){
	if(DisplayState == SelectOutVoltage){
		return;
	}
	DisplayState = SelectOutVoltage;

	
	OLED_ShowCHinese(0,2,19,0);
	OLED_ShowCHinese(16,2,0,0); //正相显示汉字“输”
	OLED_ShowCHinese(32,2,1,0);//正相显示汉字“入”
	OLED_ShowString(48,2,InVolValue[InOutKey.InKey].voltage,16, 0);
	
	OLED_ShowCHinese(0,4,20,1);
	OLED_ShowCHinese(16,4,0,1); //正相显示汉字“输”
	OLED_ShowCHinese(32,4,2,1);//正相显示汉字“出”
	OLED_ShowString(48,4,DacVolValue[InOutKey.OutKey].voltage,16, 1);
	
	OLED_ShowCHinese(0,6,21,0);
	OLED_ShowCHinese(16,6,13,0); //正相显示汉字“退”
	OLED_ShowCHinese(32,6,14,0); //正相显示汉字“出”
	OLED_ShowCHinese(48,6,6,0); //正相显示汉字“设”
	OLED_ShowCHinese(64,6,7,0);//正相显示汉字“置”
	
}


//设置输入电压
void SetIn(uint8_t is_force_update){
	static uint8_t is_blink;
	if(is_force_update){ //强制刷新数据
		is_blink++; //使其能闪烁
		if(is_blink >= 2){
			is_blink = 0;
		}
		OLED_ShowString(48,2,InVolValue[InOutKey.InKey].voltage,16,is_blink);
		return;
	}
	
	if(DisplayState == SetInVoltage){
		return;
	}
	
	
	
	OLED_ShowCHinese(0,2,19,0);
	OLED_ShowCHinese(16,2,0,0); //正相显示汉字“输”
	OLED_ShowCHinese(32,2,1,0);//正相显示汉字“入”
	OLED_ShowString(48,2,InVolValue[InOutKey.InKey].voltage,16, is_blink);
	
	
}

//设置输出电压
void SetOut(uint8_t is_force_update){
	static uint8_t is_blink;
	if(is_force_update){ //强制刷新数据
		is_blink++;//使其能闪烁
		if(is_blink >= 2){
			is_blink = 0;
		}
		OLED_ShowString(48,4,DacVolValue[InOutKey.OutKey].voltage,16, is_blink);
		return;
	}
	
	if(DisplayState == SetOutVoltage){
		return;
	}
	DisplayState = SetOutVoltage;
	OLED_ShowCHinese(0,4,20,0);
	OLED_ShowCHinese(16,4,0,0); //正相显示汉字“输”
	OLED_ShowCHinese(32,4,2,0);//正相显示汉字“出”
	OLED_ShowString(48,4,DacVolValue[InOutKey.OutKey].voltage,16, is_blink);
	
}

//退出设置
void ExitSetMenu(void){
	if(DisplayState == ExitSet){
		return;
	}
	DisplayState = ExitSet;
	
	
	OLED_ShowCHinese(0,2,19,0);
	OLED_ShowCHinese(16,2,0,0); //正相显示汉字“输”
	OLED_ShowCHinese(32,2,1,0);//正相显示汉字“入”
	OLED_ShowString(48,2,InVolValue[InOutKey.InKey].voltage,16, 0);
	
	OLED_ShowCHinese(0,4,20,0);
	OLED_ShowCHinese(16,4,0,0); //正相显示汉字“输”
	OLED_ShowCHinese(32,4,2,0);//正相显示汉字“出”
	OLED_ShowString(48,4,DacVolValue[InOutKey.OutKey].voltage,16, 0);
	
	OLED_ShowCHinese(0,6,21,1);
	OLED_ShowCHinese(16,6,13,1); //正相显示汉字“退”
	OLED_ShowCHinese(32,6,14,1); //正相显示汉字“出”
	OLED_ShowCHinese(48,6,6,1); //正相显示汉字“设”
	OLED_ShowCHinese(64,6,7,1);//正相显示汉字“置”
}


//设置成功
void SetOkMenu(void){
	static uint8_t time_num;
	if(DisplayState == SetOk){
		time_num++;
		if(time_num >= 2){
			MenuState = MaxMenu;
		}
		
		return;
	}
	DisplayState = SetOk;
	OLED_Clear(); 
	//显示“设置成功”
	OLED_ShowCHinese(32,2,6,0);
	OLED_ShowCHinese(48,2,7,0);
	OLED_ShowCHinese(64,2,15,0);
	OLED_ShowCHinese(80,2,16,0);
}


//设置下输入输出
void SetVA(void){
	CH224K_XV(InVolValue[InOutKey.InKey].Value);
	DAC_SetValue(DacVolValue[InOutKey.OutKey].Value);
}
