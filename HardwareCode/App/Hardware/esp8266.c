#include "esp8266.h"
#include "string.h"
#include "app.h"


ESP8266_State EspState;
result_data ResultData;
uint8_t HT_Time=0;

//初始化WIFI模块，使其结构体初始化
void ESP8266Init(void){
	EspState.ATCMD = INIT;
	EspState.INIT_ok = 0;
	EspState.is_TC_ok = 0;
	
	ResultData.data_len = 0;
	ResultData.is_ok = 0;
	ResultData.Delay_s = 0;
}


//Usart接收回调
void uart_call_func(uint8_t len,const char* ResData){
//	memset(ResultData.ResData,0,sizeof(ResultData.ResData));
	ResultData.is_ok = 1;
	ResultData.data_len = len;
}


//1S调用一次，按照顺序连接网络
void Esp8266_func(void){
	if(EspState.ATCMD == INIT){
		ResultData.Delay_s++;
		if(ResultData.Delay_s > 5){
			ESP_CIPSTATUS();
			return;
		}
		
		if(strstr(Uart1_DMA_Rece_Buf,"ready")){
			EspState.INIT_ok = 1;  //说明模块已经初始化完成
			return;
		}
		return;
	}

	if(ResultData.is_ok == 0){
		return; //说明没有收到任何消息
	}
	
	switch(EspState.ATCMD){
		case AT:
			if(strstr(Uart1_DMA_Rece_Buf,"OK")){
				ESP_CWMODE();
				return;
			}else{
				ESP_AT();
			}
			break;
		case CWMODE:
			if(strstr(Uart1_DMA_Rece_Buf,"OK")){
				ESP_CWJAP_DEF();
				return;
			}else{
				ESP_AT();
			}
			break;
		case CWJAP_DEF:
			if(strstr(Uart1_DMA_Rece_Buf,"OK")){
				ESP_CIPSTA();
				return;
			}else{
				ESP_CWMODE();
			}
			break;       
		case CIPSTA:
			if(strstr(Uart1_DMA_Rece_Buf,"OK")){
				ESP_CWAUTOCONN();
				return;
			}else{
				ESP_CWJAP_DEF();
			}
			break;
		case CWAUTOCONN:
			if(strstr(Uart1_DMA_Rece_Buf,"OK")){
				ESP_CIPSTART();
				return;
			}else{
				ESP_CIPSTA();
			}
			break;
		case CIPSTART:
			if(strstr(Uart1_DMA_Rece_Buf,"OK")){
				ESP_CIPMODE1();
				return;
			}else{
				ESP_CWAUTOCONN();
			}
			break;
		case CIPMODE1:
			if(strstr(Uart1_DMA_Rece_Buf,"OK")){
				ESP_CIPSEND();
				return;
			}else{
				ESP_CIPSTART();
			}
			break;
		case CIPSEND:
			if(strstr(Uart1_DMA_Rece_Buf,">")){
				EspState.is_TC_ok = 1;
				ESP_Send_HT();
				return;
			}else{
				ESP_CIPSTART();
			}
			break;
		case CIPSTATUS:
			if(strstr(Uart1_DMA_Rece_Buf,"STATUS:2")){
				EspState.CIPSTATUS_State = 2;
				ESP_CIPSTART();
				return;
			}else{
				EspState.CIPSTATUS_State = 0;
				ESP_AT();
			}
			break;
		case SendMsg:
			ESP_HandleMsg();
			break;
		case CADD3:
			if(strstr(Uart1_DMA_Rece_Buf,"CLOSED")){
				ESP_CIPMODE0();
				return;
			}else if(strstr(Uart1_DMA_Rece_Buf,"+++")){
				//说明已经退出了
				//重新初始化
				ESP8266Init();
			}
			break;
		case CIPMODE0:
			if(strstr(Uart1_DMA_Rece_Buf,"CLOSED")){
				//重新初始化
				ESP8266Init();
				return;
			}else{
				ESP_CADD3();
			}
			break;
		default:
			break;
	}
	memset(Uart1_DMA_Rece_Buf,0,sizeof(Uart1_DMA_Rece_Buf));
}



//解析服务端回发的消息
void ESP_HandleMsg(void){
	int a,b;
	if(strstr(Uart1_DMA_Rece_Buf,"_")){   //判断是否是远程设置参数
		printf("%s \r\n",Uart1_DMA_Rece_Buf);	
		sscanf(Uart1_DMA_Rece_Buf, "%d_%d",&a,&b);
		InOutKey.InKey = (uint8_t)a;
		InOutKey.OutKey = (uint8_t)b;
		
		SetVA();
	}
	

}



//检测连接状态 STATUS:2 获得ip   3:建立连接  4:失去连接 
void ESP_CIPSTATUS(void){
	EspState.ATCMD = CIPSTATUS;
	ResultData.is_ok = 0;
	HT_Time = 0;
	USART_Send(USART1,"AT+CIPSTATUS\r\n");
}


//发送+++ 准备退出透传模式
void ESP_Add3(void){
	EspState.ATCMD = CADD3;
	ResultData.is_ok = 0;
	HT_Time = 0;
	USART_Send(USART1,"+++");
}


//发送AT命令
void ESP_AT(void){
	EspState.ATCMD = AT;
	ResultData.is_ok = 0;
	HT_Time = 0;
	USART_Send(USART1,"AT\r\n");
}

//设置为Station 模式：AT+CWMODE=1
void ESP_CWMODE(void){
	EspState.ATCMD = CWMODE;
	ResultData.is_ok = 0;
	HT_Time = 0;
	USART_Send(USART1,"AT+CWMODE=1\r\n");
}

//连接WIFI
void ESP_CWJAP_DEF(void){
	char *cStr;
	EspState.ATCMD = CWJAP_DEF;
	ResultData.is_ok = 0;
	HT_Time = 0;
	sprintf ( cStr,"AT+CWJAP_DEF=\"%s\",\"%s\"\r\n",wifi_name,wifi_pw );
	USART_Send(USART1,cStr);
}

//获取 IP 地址
void ESP_CIPSTA(void){
	EspState.ATCMD = CIPSTA;
	ResultData.is_ok = 0;
	HT_Time = 0;
	USART_Send(USART1,"AT+CIPSTA?\r\n");
}

//置模块开机自动连接热点：AT+CWAUTOCONN=1
void ESP_CWAUTOCONN(void){
	EspState.ATCMD = CWAUTOCONN;
	ResultData.is_ok = 0;
	HT_Time = 0;
	USART_Send(USART1,"AT+CWAUTOCONN=1\r\n");
}

//连接TCP
void ESP_CIPSTART(void){
	char cStr [100] = { 0 };
	EspState.ATCMD = CIPSTART;
	ResultData.is_ok = 0;
	HT_Time = 0;
	sprintf(cStr,"AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",TCPServer_IP,TCPServer_PORT );
	USART_Send(USART1,cStr);
}

//开启透传
void ESP_CIPMODE1(void){
	EspState.ATCMD = CIPMODE1;
	ResultData.is_ok = 0;
	HT_Time = 0;
	USART_Send(USART1,"AT+CIPMODE=1\r\n");
}

//准备发送透传消息
void ESP_CIPSEND(void){
	EspState.ATCMD = CIPSEND;
	ResultData.is_ok = 0;
	HT_Time = 0;
	USART_Send(USART1,"AT+CIPSEND\r\n");
}


//发送心跳信息
void ESP_Send_HT(void){
	HT_Time++;   //心跳时间加1
	
	if(EspState.is_TC_ok == 0){
		return;
	}
	
	if(HT_Time >= Heartbeat_Time){
		if(ResultData.is_ok == 0){
			//说明很久没有收到回复了，可能连接断开了
			ESP_CADD3();
		}
		ESP_SendMsg("ping#");
		HT_Time = 0;
	}
}


//发送透传消息
void ESP_SendMsg(char* Str){
	if(EspState.is_TC_ok == 0){
		return;
	}
	
	EspState.ATCMD = SendMsg;
	ResultData.is_ok = 0;
	HT_Time = 0;
	
//	char cStr [100] = { 0 };
//	sprintf(cStr,"%s", Str);
	USART_Send(USART1,Str);
}

//准备退出透传
void ESP_CADD3(void){
	EspState.ATCMD = CADD3;
	ResultData.is_ok = 0;
	HT_Time = 0;
	USART_Send(USART1,"+++");
}

//退出透传
void ESP_CIPMODE0(void){
	EspState.ATCMD = CIPMODE0;
	ResultData.is_ok = 0;
	HT_Time = 0;
	USART_Send(USART1,"AT+CIPMODE=0\r\n");
}
