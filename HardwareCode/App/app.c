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


menu_state MenuState;  //�˵��Ķ���
menu_state DisplayState;  //��ʾ�˵��Ķ���  ��ֹ��Ļ����ˢ��
in_out_key InOutKey;

int8_t MaxMenuKey = 0;

//�����ѹ���ݵĶ���
show_value DacVolValue[24] = {
	{4095,":Min  "},{3880,":2V   "},{3800,":3V   "},{3720,":4V   "},{3705,":4.2V "},{3640,":5V   "},{3560,":6V   "},{3475,":7V   "},
	{3390,":8V   "},{3360,":8.4V "},{3350,":9V   "},{3220,":10V  "},{3135,":11V  "},{3055,":12V  "},{3000,":12.6V"},{2970,":13V  "},
	{2880,":14V  "},{2790,":15V  "},{2700,":16V  "},{2630,":16.8V"},{2610,":17V  "},{2520,":18V  "},{2420,":19V  "},{0,":Max  "}
};

//�����ѹ�Ķ���
show_value InVolValue[5] = {
	{5,":5V "},{9,":9V "},{12,":12V"},{15,":15V"},{20,":20V"}
};


void App_init(void){
	RCC_ClkConfiguration();
//	GPIO_PinRemapConfig(GPIO_Remap_SWJJTAGDISABLE, ENABLE);
	Delay_Init();				//��ʱ��ʼ��
	TIM_Configuration();
	UART3_Configuration(115200); //Ĭ��Ϊ���ڣ�������115200
	UART1_Configuration(115200);
	GPIO_Configuration();   //led�ĳ�ʼ��
	
	InOutKey.InKey = 0;
	InOutKey.OutKey = 0;
	
	Delay_Ms(200);//�ȴ�ģ��
	
	I2C_IO_Init();//I2C��ʼ��

	log_clocks();
	
	Ch224k_Init();

	KeyIO_Init();
	Software_SPI_Init();
	
	
	DAC_Configuration(); //dac��ʼ��
	
	ADC_Configuration();//ADC��ʼ��
	
  //oled�ĳ�ʼ��
   OLED_Init();                           //OLED��ʼ
   OLED_Clear();                         //����
	
	ESP8266Init();
	
	Delay_Ms(200);
	DisplayState = VoidMenu;
	MenuState = MaxMenu;
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // ����ADC1���ת��
	DAC_SetValue(4095);
}


void _0ms_func(void){}
	

void _1ms_func(void){
	
}
	

void _10ms_func(void){
	KeyInputScan();
	KeyValueCalculate();
	
	switch (MenuState){
		case MaxMenu: //��ʼ�˵�
			Menu_Init();
			if(L_KeyState(PROBE2_KEY)){
				//����set   ��������ģʽ
				MenuState = SelectInVoltage;
				OLED_Clear();//����
			}
			
			if(R_KeyState(PROBE1_KEY)){ //set�½���
				MaxMenuKey = -2;
				DisplayState = VoidMenu;
				return;
			}
			if(R_KeyState(PROBE3_KEY)){ //set�½���
				MaxMenuKey = 0;
				DisplayState = VoidMenu;
				return;
			}
			
			
		break;
			
		case SelectInVoltage:   //ѡ�������ѹ
			SelectIn();
			if(R_KeyState(PROBE2_KEY)){ //set�½���
				MenuState = SetInVoltage;
				return;
			}
			if(R_KeyState(PROBE1_KEY)){ //����<
				MenuState = ExitSet;
				return;
			}
			if(R_KeyState(PROBE3_KEY)){ //����>
				MenuState = SelectOutVoltage;
				return;
			}
		break;
		
		case SelectOutVoltage:  //ѡ�������ѹ
			SelectOut();
			if(R_KeyState(PROBE2_KEY)){ //set�½���
				MenuState = SetOutVoltage;
				return;
			}
			if(R_KeyState(PROBE1_KEY)){ //����<
				MenuState = SelectInVoltage;
				return;
			}
			if(R_KeyState(PROBE3_KEY)){ //����>
				MenuState = ExitSet;
				return;
			}
			break;
			
		case ExitSet:  //�˳�����
			ExitSetMenu();
			if(R_KeyState(PROBE2_KEY)){ //set�½���
				MenuState = MaxMenu;
				return;
			}
			if(R_KeyState(PROBE1_KEY)){ //����<
				MenuState = SelectOutVoltage;
				return;
			}
			if(R_KeyState(PROBE3_KEY)){ //����>
				MenuState = SelectInVoltage;
				return;
			}
			break;
		
		case SetInVoltage:   //���������ѹ
			SetIn(0);
			if(L_KeyState(PROBE2_KEY)){   //����set ����
				MenuState = SelectInVoltage;
			}
			
			if(D_KeyState(PROBE1_KEY)){ //����<
				if(InOutKey.InKey <= 0){
					InOutKey.InKey = 4;
					SetIn(1);
					return;
				}
				InOutKey.InKey--;
				SetIn(1);
				return;
			}
			if(D_KeyState(PROBE3_KEY)){ //����>
				if(InOutKey.InKey >= 4){
					InOutKey.InKey = 0;
					SetIn(1);
					return;
				}
				InOutKey.InKey++;
				SetIn(1);
				return;
			}
			if(R_KeyState(PROBE2_KEY)){ //����set
				MenuState = SetOk;
				SetOkMenu();
				//���þ���ĵ�ѹ
				CH224K_XV(InVolValue[InOutKey.InKey].Value);
				return;
			}
			break;
		
		case SetOutVoltage:    //���������ѹ
			SetOut(0);
			if(L_KeyState(PROBE2_KEY)){   //����set ����
				MenuState = SelectOutVoltage;
			}
			if(D_KeyState(PROBE1_KEY)){ //����<
				if(InOutKey.OutKey <= 0){
					InOutKey.OutKey = 23;
					SetOut(1);
					return;
				}
				InOutKey.OutKey--;
				SetOut(1);
				return;
			}
			if(D_KeyState(PROBE3_KEY)){ //����>
				if(InOutKey.OutKey >= 23){
					InOutKey.OutKey = 0;
					SetOut(1);
					return;
				}
				InOutKey.OutKey++;
				SetOut(1);
				return;
			}
			if(R_KeyState(PROBE2_KEY)){ //����set
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
	GPIO_Turn(GPIOB,GPIO_Pin_12); //״̬����˸
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // ����ADC1���ת��
	ADC_SoftwareStartConvCmd(ADC2, ENABLE); // ����ADC1���ת��
	
	
	if(MenuState == SetOk){
		//ʹ��ɹ����沥��һ��
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
	
	

	Esp8266_func();   //8266��������ʼ��
	
	
	//�������ݸ������
	//�������룬��������������ѹ���������
	sprintf(str,"%d,%d,%s,%s,#",InOutKey.InKey,InOutKey.OutKey,VA[0],VA[1]);
	ESP_SendMsg(str);

}



//��ʼ���˵�
void Menu_Init(void){
	if(DisplayState == MaxMenu){
		return;
	}
	DisplayState = MaxMenu;


	OLED_ShowCHinese(0,0,0,0); //������ʾ���֡��䡱
	OLED_ShowCHinese(16,0,2,0);//������ʾ���֡�����
	OLED_ShowCHinese(32,0,3,0);//������ʾ���֡��硱
	OLED_ShowCHinese(48,0,4,0);//������ʾ���֡�ѹ��
	OLED_ShowString(64,0,VA[0],16, 0);


	
	//�������
	OLED_ShowCHinese(0,2,0,0);
	OLED_ShowCHinese(16,2,2,0);
	OLED_ShowCHinese(32,2,3,0);
	OLED_ShowCHinese(48,2,5,0);
	OLED_ShowString(64,2,VA[1],16, 0);
	
	
	
	//��������
	OLED_ShowCHinese(0,4,6,0);
	OLED_ShowCHinese(16,4,7,0);
	OLED_ShowCHinese(32,4,0,0);
	OLED_ShowCHinese(48,4,1,0);
	OLED_ShowString(64,4,InVolValue[InOutKey.InKey].voltage,16, 0);
	
	
	//�������
	OLED_ShowCHinese(0,6,6,0);
	OLED_ShowCHinese(16,6,7,0);
	OLED_ShowCHinese(32,6,0,0);
	OLED_ShowCHinese(48,6,2,0);
	OLED_ShowString(64,6,DacVolValue[InOutKey.OutKey].voltage,16, 0);
}


//ѡ�������ѹ
void SelectIn(void){	
	
	if(DisplayState == SelectInVoltage){
		return;
	}
	DisplayState = SelectInVoltage;
	
	OLED_ShowCHinese(0,0,23,0);
	OLED_ShowCHinese(16,0,6,0); //������ʾ���֡��衱
	OLED_ShowCHinese(32,0,7,0);//������ʾ���֡��á�
	OLED_ShowCHinese(48,0,8,0);//������ʾ���֡�ģ��
	OLED_ShowCHinese(64,0,9,0);//������ʾ���֡�ʽ��
	
	
	OLED_ShowCHinese(0,2,19,1);
	OLED_ShowCHinese(16,2,0,1); //������ʾ���֡��䡱
	OLED_ShowCHinese(32,2,1,1);//������ʾ���֡��롱
	OLED_ShowString(48,2,InVolValue[InOutKey.InKey].voltage,16, 1);
	
	OLED_ShowCHinese(0,4,20,0);
	OLED_ShowCHinese(16,4,0,0); //������ʾ���֡��䡱
	OLED_ShowCHinese(32,4,2,0);//������ʾ���֡�����
	OLED_ShowString(48,4,DacVolValue[InOutKey.OutKey].voltage,16, 0);
	
	OLED_ShowCHinese(0,6,21,0);
	OLED_ShowCHinese(16,6,13,0); //������ʾ���֡��ˡ�
	OLED_ShowCHinese(32,6,14,0); //������ʾ���֡�����
	OLED_ShowCHinese(48,6,6,0); //������ʾ���֡��衱
	OLED_ShowCHinese(64,6,7,0);//������ʾ���֡��á�
}


//ѡ�������ѹ
void SelectOut(void){
	if(DisplayState == SelectOutVoltage){
		return;
	}
	DisplayState = SelectOutVoltage;

	
	OLED_ShowCHinese(0,2,19,0);
	OLED_ShowCHinese(16,2,0,0); //������ʾ���֡��䡱
	OLED_ShowCHinese(32,2,1,0);//������ʾ���֡��롱
	OLED_ShowString(48,2,InVolValue[InOutKey.InKey].voltage,16, 0);
	
	OLED_ShowCHinese(0,4,20,1);
	OLED_ShowCHinese(16,4,0,1); //������ʾ���֡��䡱
	OLED_ShowCHinese(32,4,2,1);//������ʾ���֡�����
	OLED_ShowString(48,4,DacVolValue[InOutKey.OutKey].voltage,16, 1);
	
	OLED_ShowCHinese(0,6,21,0);
	OLED_ShowCHinese(16,6,13,0); //������ʾ���֡��ˡ�
	OLED_ShowCHinese(32,6,14,0); //������ʾ���֡�����
	OLED_ShowCHinese(48,6,6,0); //������ʾ���֡��衱
	OLED_ShowCHinese(64,6,7,0);//������ʾ���֡��á�
	
}


//���������ѹ
void SetIn(uint8_t is_force_update){
	static uint8_t is_blink;
	if(is_force_update){ //ǿ��ˢ������
		is_blink++; //ʹ������˸
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
	OLED_ShowCHinese(16,2,0,0); //������ʾ���֡��䡱
	OLED_ShowCHinese(32,2,1,0);//������ʾ���֡��롱
	OLED_ShowString(48,2,InVolValue[InOutKey.InKey].voltage,16, is_blink);
	
	
}

//���������ѹ
void SetOut(uint8_t is_force_update){
	static uint8_t is_blink;
	if(is_force_update){ //ǿ��ˢ������
		is_blink++;//ʹ������˸
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
	OLED_ShowCHinese(16,4,0,0); //������ʾ���֡��䡱
	OLED_ShowCHinese(32,4,2,0);//������ʾ���֡�����
	OLED_ShowString(48,4,DacVolValue[InOutKey.OutKey].voltage,16, is_blink);
	
}

//�˳�����
void ExitSetMenu(void){
	if(DisplayState == ExitSet){
		return;
	}
	DisplayState = ExitSet;
	
	
	OLED_ShowCHinese(0,2,19,0);
	OLED_ShowCHinese(16,2,0,0); //������ʾ���֡��䡱
	OLED_ShowCHinese(32,2,1,0);//������ʾ���֡��롱
	OLED_ShowString(48,2,InVolValue[InOutKey.InKey].voltage,16, 0);
	
	OLED_ShowCHinese(0,4,20,0);
	OLED_ShowCHinese(16,4,0,0); //������ʾ���֡��䡱
	OLED_ShowCHinese(32,4,2,0);//������ʾ���֡�����
	OLED_ShowString(48,4,DacVolValue[InOutKey.OutKey].voltage,16, 0);
	
	OLED_ShowCHinese(0,6,21,1);
	OLED_ShowCHinese(16,6,13,1); //������ʾ���֡��ˡ�
	OLED_ShowCHinese(32,6,14,1); //������ʾ���֡�����
	OLED_ShowCHinese(48,6,6,1); //������ʾ���֡��衱
	OLED_ShowCHinese(64,6,7,1);//������ʾ���֡��á�
}


//���óɹ�
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
	//��ʾ�����óɹ���
	OLED_ShowCHinese(32,2,6,0);
	OLED_ShowCHinese(48,2,7,0);
	OLED_ShowCHinese(64,2,15,0);
	OLED_ShowCHinese(80,2,16,0);
}


//�������������
void SetVA(void){
	CH224K_XV(InVolValue[InOutKey.InKey].Value);
	DAC_SetValue(DacVolValue[InOutKey.OutKey].Value);
}
