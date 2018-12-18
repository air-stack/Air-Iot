# Air Iot

> 基于指南者Stm32开发板进行开发，通过传感器感知大气质量指标，通过WIFI模块进行TCP通信，完成物联网嵌入式设备的构建。

## 技术栈

> Stm32F103芯片 + WIFI ESP8266模块 + Sensor传感器 (DHT11温湿度传感器)

## IDE 

> Keil Mdk 5

> CLion

## 网络调试

1. 运行网络调试工具 NetAssist.exe ， 直接查看TCP接收数据信息
2. 运行服务端程序 **[Air-Server](https://github.com/kevinten10/Air-Server)**，完成数据的网络通信流程

## 配置说明

请到 User/Test/test.h 配置文件中，修改WIFI账号密码 以及相关端口信息

请到 User/Test/test.c 配置文件中，修改数据协议

## 运行流程

1. 温湿度传感器DHT11 + PM2.5传感器 连接到开发板
2. 数据通过串口传输到MCU，在MAIN函数进行编解码编程操作
3. WIFI ESP8266 模块通过TCP与服务端通信

## 实验现象

![传感器配置成功](实验现象/传感器配置.png)

![传感器未配置](实验现象/传感器未配置.png)