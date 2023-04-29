
#ifndef __ESP8266_H__
#define __ESP8266_H__


#include "air32f10x.h"
#include "uart.h"

//ATE1 �C �򿪻���   ATE0 �C �رջ���


typedef enum {
	INIT,//�ȴ���ʼ��
	AT,//����ģ���Ƿ�����
	CWMODE, //����ΪStation ģʽ��AT+CWMODE=1
	CWJAP_DEF,//���ӵ�ָ��·������wifi����wifi����
	CIPSTA, //��ȡ IP ��ַ��AT+CIPSTA?
	CWAUTOCONN,//����ģ�鿪���Զ������ȵ㣺AT+CWAUTOCONN=1��ģ���ϵ��ͻ��Զ�����֮ǰ���ӹ���·�������洢�����ֺ����룩
	CWLAP,//ɨ�踽���� AP �ȵ���Ϣ��AT+CWLAP����������Ϣ�У��ȵ������ȵ� MAC ��ַ�����ܷ�ʽ�ȣ�
	CWQAP,//�Ͽ����ӣ�AT+CWQAP
	CIPSTART,//����TCP  AT+CIPSTART="TCP","192.168.0.107",8080
	CIPMODE1,//ʹ�� AT+CIPMODE=1 ָ���͸��ģʽ
	CADD3, //����+++��׼���˳�͸��ģʽ
	CIPMODE0,//���� +++ �������ַ���ֵ��ע����Ǹ�����û�л��з�,���ŷ��� AT+CIPMODE=0 ������˳�͸��ģʽ�����ص�������ģʽ
	CIPSEND,//����͸��ģʽ��ֻ��Ҫ�ڵ�һ�η�������ʱʹ�� AT+CIPSEND ָ�������� ESP8266 ��ʼ͸������
	CIPSEND_N,//���͵���TCP���ݣ�AT+CIPSEND=10������10ָ������η������ݵĳ��ȡ��������ֽ�����ʾ >��Ȼ�������ݼ��ɣ�
	CIPSTATUS,//�������״̬ STATUS:2 ���ip   3:��������   4:ʧȥ���� 
	SendMsg,//����͸����Ϣ
}AT_CMD;


//ÿ���������ݵ�״̬
typedef struct{
	AT_CMD ATCMD;
	uint8_t INIT_ok;//��ʼ��
	uint8_t is_TC_ok;//͸��״̬ 1����   0Ϊ����
	uint8_t CIPSTATUS_State;//��������״̬
}ESP8266_State;


//�յ����ݵ�ָʾ
typedef struct{
	uint8_t data_len; //�������ݵĳ���
	const char *ResData; //���յ�����ָ��
	uint8_t is_ok; //�Ƿ��Ѿ��յ��ı�־
	uint8_t Delay_s;//�ϵ�ȴ�5s   ��֤�Ƿ��Զ�����wifi
}result_data;



#define wifi_name  "XXXX"          //wifi��
#define wifi_pw    "XXXXX"      //wifi����

#define TCPServer_IP     "XXXXXX"     //������IP
#define TCPServer_PORT   "8999"      //�������˿ں�

#define Heartbeat_Time   10 //��������ʱ��

void ESP8266Init(void);
void uart_call_func(uint8_t len,const char* ResData);
void ESP_AT(void);
void ESP_CIPSTATUS(void);
void ESP_CWMODE(void);
void ESP_CWJAP_DEF(void);
void ESP_CIPSTA(void);
void ESP_CWAUTOCONN(void);
void ESP_CIPSTART(void);
void ESP_CIPMODE1(void);
void ESP_CIPSEND(void);
void ESP_SendMsg(char* Str);
void ESP_Send_HT(void);
void ESP_CADD3(void);
void ESP_CIPMODE0(void);
void ESP_HandleMsg(void);
void Esp8266_func(void);
#endif
