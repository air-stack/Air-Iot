#include "test.h"
#include "bsp_esp8266.h"
#include "bsp_SysTick.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "bsp_dht11.h"
#include "bsp_gp2y.h"

volatile uint8_t ucTcpClosedFlag = 0;

/**
  * @brief  ESP8266 （Sta Tcp Client）透传
  * @param  无
  * @retval 无
  */
void ESP8266_StaTcpClient_UnvarnishTest(void) {
    uint8_t ucStatus;

    char cStr[100] = {0};
		
		// IMEI 15BYTE
		char imei[30] = {'1','1','2','2','3','3','4','4','5','5','6','6','7','7','8','8','9','9','A','A','B','B','C','C','D','D','E','E','F','F'};

    // DHT11数据结构体
    DHT11_Data_TypeDef DHT11_Data;
    // PM25数据结构体
    PM_Data_TypeDef PM_Data;

    printf("\r\n正在配置 ESP8266 WIFI模块......\r\n");

    macESP8266_CH_ENABLE();

    ESP8266_AT_Test();

    ESP8266_Net_Mode_Choose(STA);

    while (!ESP8266_JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd));

    ESP8266_Enable_MultipleId(DISABLE);

    while (!ESP8266_Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0));

    while (!ESP8266_UnvarnishSend());

    printf("\r\n配置 ESP8266 完毕\r\n");

    // 数据循环读取发送
    while (1) {

        // FIXME 读取PM25传感器模块数据
        if (PM_Read_DATA(&PM_Data) == SUCCESS) {
            sprintf(cStr,
					"G{imei:\"%s\",start:\"%d\",voutH:\"%d\",voutL:\"%d\",vrefH:\"%d\",vrefL:\"%d\",check:\"%d\",finish:\"%d\"}",
                   imei,PM_Data.start,PM_Data.voutH, PM_Data.voutL, PM_Data.vrefH, PM_Data.vrefL, PM_Data.check_sum, PM_Data.finish);
        } else
            sprintf(cStr, "PM25 Monitoring failure...");

        // 打印待发送数据到WIFI消息队列
        printf("%s", cStr);
        // 通过WIFI发送数据字符串
        ESP8266_SendString(ENABLE, cStr, 0, Single_ID_0);

        // FIXME 读取DHT11温湿度传感器数据
        if (DHT11_Read_TempAndHumidity(&DHT11_Data) == SUCCESS) {
            sprintf(cStr,
					"D{imei:\"%s\",tempInt:\"%d\",tempDeci:\"%d\",humiInt:\"%d\",humiDeci:\"%d\"}",
                    imei,DHT11_Data.humi_int, DHT11_Data.humi_deci, DHT11_Data.temp_int, DHT11_Data.temp_deci);
        } else
            sprintf(cStr, "DHT11 Monitoring failure...");


        // 打印待发送数据到WIFI消息队列
        printf("%s", cStr);
        // 通过WIFI发送数据字符串
        ESP8266_SendString(ENABLE, cStr, 0, Single_ID_0);

        Delay_ms(1000);

        // 检测是否失去连接
        if (ucTcpClosedFlag) {
            // 退出透传模式
            ESP8266_ExitUnvarnishSend();

            // 获取连接状态
            do ucStatus = ESP8266_Get_LinkStatus();
            while (!ucStatus);

            // 确认失去连接后重连
            if (ucStatus == 4) {
                printf("\r\n正在重连热点和服务器 ......\r\n");

                while (!ESP8266_JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd));

                while (!ESP8266_Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port,
                                            Single_ID_0));

                printf("\r\n重连热点和服务器成功\r\n");

            }

            while (!ESP8266_UnvarnishSend());

        }

    }

}


