#ifndef __WIFI_CONFIG_H
#define	__WIFI_CONFIG_H


#include "stm32f10x.h"


#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


//* 数据类型 */
typedef struct                                                     //定义配置WiFi模块的指令集的结构体类型
{
	char * netmode;
	char * dhcpd;
	char * dhcpd_ip;
	char * dhcpd_dns;
	char * wifi_conf;
	char * dhcpc;
	char * net_ip;
	char * net_dns;
	char * remotepro;
	char * mode;
	char * remoteip;
	char * remoteport;
	char * CLport;   
	char * timeout;                                                  //默认值
	char * uart;                                                     //默认值  
	char * uartpacklen;                                              //默认值
	char * uartpacktimeout;  	                                       //默认值
} STRUCT_Net_ConfigTypeDef;


#define RX_BUF_MAX_LEN     1024                                    //最大接收缓存字节数

extern struct  STRUCT_USARTx_Fram                                  //串口数据帧的处理结构体
{
	u8  Data_RX_BUF[ RX_BUF_MAX_LEN ];
	
  union {
    __IO u16 InfAll;
    struct {
		  __IO u16 FramLength       :15;                               // 14:0 
		  __IO u16 FramFinishFlag   :1;                                // 15 
	  } InfBit;
  }; 
	
} strUSART1_Fram_Record, strUSART2_Fram_Record;


//* 函数 */
#define WiFi_ES_INIT                   GPIO_Config                //WiFi使用的ES引脚初始化函数，推挽输出 
#define WiFi_USART1_INIT               USART1_Config              //WiFi使用的串口1初始化函数，波特率115200
#define WiFi_USART2_INIT               USART2_Config              //WiFi使用的串口2初始化函数，波特率115200 
#define WiFi_TIM2_INIT                 TIM2_Configuration
#define WiFi_NVIC_INIT                 NVIC_Configuration         //NVIC中断配置

void WiFi_Config( void );
void NVIC_Configuration( void );


#endif    /* __WIFI_CONFIG_H */
