#ifndef  __TEST_H
#define     __TEST_H

#include "stm32f10x.h"

/** TODO 修改WIFI配置信息 */

/********************************** 用户需要设置的参数 **********************************/
// 要连接的热点的名称，即WIFI名称
#define      macUser_ESP8266_ApSsid           "ten2"

// 要连接的热点的密钥
#define      macUser_ESP8266_ApPwd            "wangshihao"

// 要连接的服务器的IP，即电脑的IP，Windows请使用ipconfig命令查看
#define      macUser_ESP8266_TcpServer_IP     "47.100.236.64"

// 要连接的服务器的端口
#define      macUser_ESP8266_TcpServer_Port    "2759"


/********************************** 外部全局变量 ***************************************/
extern volatile uint8_t ucTcpClosedFlag;


/********************************** 测试函数声明 ***************************************/
void ESP8266_StaTcpClient_UnvarnishTest(void);


#endif

