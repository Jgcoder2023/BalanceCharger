#ifndef __APP_H__
#define __APP_H__

#include "air32f10x.h"
#include "air32f10x_rcc.h"



//���˵���ǰ��״̬
typedef enum{
	VoidMenu,//�ղ˵�
	MaxMenu, //��ʼ�˵�
	SelectInVoltage, //ѡ�������ѹ
	SelectOutVoltage, //ѡ�������ѹ
	SetInVoltage, //���������ѹ
	SetOutVoltage,  //���������ѹ
	ExitSet, //�˳�����
	SetOk //���óɹ�����
}menu_state;

//�����������״̬
typedef enum{
	Input,
	Output
}setVoltage;


//���������ѹ��DAC�Ķ�Ӧ
//���������ѹ��ֵ�Ķ�Ӧ
//��ʾֵ��ʵ��ֵ��Ӧ
typedef struct{
	uint16_t Value;
	char* voltage;
}show_value;


//����һ����¼�����������ѹ��key
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

