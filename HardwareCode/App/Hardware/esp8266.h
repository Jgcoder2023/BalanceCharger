
#ifndef __ESP8266_H__
#define __ESP8266_H__


#include "air32f10x.h"
#include "uart.h"

//ATE1 – 打开回显   ATE0 – 关闭回显


typedef enum {
	INIT,//等待初始化
	AT,//测试模块是否正常
	CWMODE, //设置为Station 模式：AT+CWMODE=1
	CWJAP_DEF,//连接到指定路由器：wifi名和wifi密码
	CIPSTA, //获取 IP 地址：AT+CIPSTA?
	CWAUTOCONN,//设置模块开机自动连接热点：AT+CWAUTOCONN=1，模块上电后就会自动连接之前连接过的路由器（存储过名字和密码）
	CWLAP,//扫描附近的 AP 热点信息：AT+CWLAP，包括的信息有（热点名，热点 MAC 地址，加密方式等）
	CWQAP,//断开连接：AT+CWQAP
	CIPSTART,//连接TCP  AT+CIPSTART="TCP","192.168.0.107",8080
	CIPMODE1,//使用 AT+CIPMODE=1 指令开启透传模式
	CADD3, //发送+++，准备退出透传模式
	CIPMODE0,//发送 +++ 这三个字符，值得注意的是该命令没有换行符,接着发送 AT+CIPMODE=0 命令即可退出透传模式，即回到正常的模式
	CIPSEND,//开启透传模式后只需要在第一次发送数据时使用 AT+CIPSEND 指令来告诉 ESP8266 开始透传发送
	CIPSEND_N,//发送单词TCP数据，AT+CIPSEND=10，其中10指的是这次发送数据的长度。串口助手界面显示 >，然后发送数据即可：
	CIPSTATUS,//检测连接状态 STATUS:2 获得ip   3:建立连接   4:失去连接 
	SendMsg,//发送透传消息
}AT_CMD;


//每次连接数据的状态
typedef struct{
	AT_CMD ATCMD;
	uint8_t INIT_ok;//初始化
	uint8_t is_TC_ok;//透传状态 1开启   0为开启
	uint8_t CIPSTATUS_State;//网络连接状态
}ESP8266_State;


//收到数据的指示
typedef struct{
	uint8_t data_len; //接收数据的长度
	const char *ResData; //接收的数据指针
	uint8_t is_ok; //是否已经收到的标志
	uint8_t Delay_s;//上电等待5s   验证是否自动连接wifi
}result_data;



#define wifi_name  "XXXX"          //wifi名
#define wifi_pw    "XXXXX"      //wifi密码

#define TCPServer_IP     "XXXXXX"     //服务器IP
#define TCPServer_PORT   "8999"      //服务器端口号

#define Heartbeat_Time   10 //设置心跳时间

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
