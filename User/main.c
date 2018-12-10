/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2014-12-xx
  * @brief   WiFi模块测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
 
#include "stm32f10x.h"
#include "bsp_gpio.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "wifi_config.h"
#include "wifi_function.h"
#include "bsp_SysTick.h"


#define NULL                   0
#define NET_MODE_TEST          1                                                  //NET_MODE_TEST = 1   ，进行以太网模式测试  
                                                                                  //              = 2   ，进行无线STA模式测试  
                                                                                  //              = 3   ，进行无线AP模式测试 
                                                                                  //              = 其他，不测试 		
																																									

STRUCT_Net_ConfigTypeDef  Net_Test_ConfigStructure =
{
	#if NET_MODE_TEST == 1                                                          //以太网模式测试               
		"at+netmode=1",                                                               //以太网模式
		NULL,                                                                         //该模式下可以不配置
		NULL,
		NULL,
		NULL,
		"at+dhcpc=0",                                                                 //关闭DHCP客户端，手动设置IP
		"at+net_ip=192.168.11.254,255.255.255.0,192.168.11.1",                        //手动设置的本地IP,子网掩码，网关
		"at+net_dns=192.168.11.1,0.0.0.0",                                            //手动设置的本地主要DNS，备用DNS
		"at+remotepro=tcp",                                                           //TCP协议
		"at+mode=server",                                                             //服务器
		"at+remoteip=192.168.1.104",                                                  //模块做客户端时要连接的服务器IP
		"at+remoteport=8080",                                                         //服务器端口
		"at+CLport= 13",                                                              //模块做客户端时的端口
		"at+timeout=0",                                                               //默认值
		"at+uart=115200,8,n,1",                                                       //默认值
		"at+uartpacklen=64",                                                          //默认值
		"at+uartpacktimeout=10"                                                       //默认值		
	
	#elif NET_MODE_TEST == 2                                                        //无线STA模式测试  
		"at+netmode=2",                                                               //无线STA模式
		NULL,
		NULL,
		NULL,
		"at+wifi_conf=wildfire,wpawpa2_aes,wildfire",                                 //要连接的WiFi网络的名称，加密方式，密钥
		"at+dhcpc=1",                                                                 //开启DHCP客户端，模块自动从路由器获取IP
		"at+net_ip=192.168.11.254,255.255.255.0,192.168.11.1",                      
		"at+net_dns=192.168.11.1,0.0.0.0",                                        
		"at+remotepro=tcp",
		"at+mode=server",
		"at+remoteip=192.168.1.104",
		"at+remoteport=8080",
		"at+CLport= 13",   
		"at+timeout=0",                                                               //默认值
		"at+uart=115200,8,n,1",                                                       //默认值
		"at+uartpacklen=64",                                                          //默认值
		"at+uartpacktimeout=10"                                                       //默认值		
  
	#elif NET_MODE_TEST == 3                                                        //无线AP模式测试  
		"at+netmode=3",                                                               //无线AP模式
		"at+dhcpd=1",                                                                 //开启DHCP服务器，自动分配IP给其他WiFi设备
		"at+dhcpd_ip=192.168.11.100,192.168.11.200,255.255.255.0,192.168.11.1",       //模块分配的IP范围，子网掩码，网关
		"at+dhcpd_dns=192.168.11.1,0.0.0.0",                                          //模块分配的主要DNS，备用DNS
		"at+wifi_conf=wildfire2,wpawpa2_aes,wildfire",                                //创建的WiFi接入点的名称，加密方式，密钥
		NULL,                                                                         
		"at+net_ip=192.168.11.254,255.255.255.0,192.168.11.1",                        //本地IP,子网掩码，网关
		"at+net_dns=192.168.11.1,0.0.0.0",                                            //本地主要DNS，备用DNS
		"at+remotepro=tcp",
		"at+mode=server",
		"at+remoteip=192.168.1.104",
		"at+remoteport=8080",
		"at+CLport= 13",   
		"at+timeout=0",                                                               //默认值
		"at+uart=115200,8,n,1",                                                       //默认值
		"at+uartpacklen=64",                                                          //默认值
		"at+uartpacktimeout=10"                                                       //默认值	
	
	#else                                                                           //不测试
	  NULL
	
  #endif	
	
};


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
	/* 初始化 */
	WiFi_Config();                                                                  //初始化WiFi模块使用的接口和外设
	SysTick_Init();                                                                 //配置 SysTick 为 1ms 中断一次 

	printf("\r\n野火 HLK-RMO4 WiFi模块测试例程\r\n");                                //打印测试例程提示信息

	#if NET_MODE_TEST                                                               
	 hlk_rm04_quit_trans();                                                         //让WiFi模块退出透传模式
	 hlk_rm04_net_config( & Net_Test_ConfigStructure );                             //配置WiFi模块进行测试
	#endif

	for(;;);
	
}


/*********************************************END OF FILE**********************/
