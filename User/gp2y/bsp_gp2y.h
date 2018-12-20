#ifndef __GP2Y_H
#define    __GP2Y_H


#include "stm32f10x.h"

/************************** PM25 数据类型定义********************************/

// 请参考 弘成基GP2Y1051AU0F仕様書20141031.pdf
typedef struct {
    uint8_t start;        // 起始位
    uint8_t voutH;
    uint8_t voutL;
    uint8_t vrefH;
    uint8_t vrefL;
    uint8_t check_sum;     // 校验和
    uint8_t finish;        // 结束位

} PM_Data_TypeDef;

#define      pm_Dout_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      pm_Dout_GPIO_CLK                       RCC_APB2Periph_GPIOE

// FIXME 使用PE5端口进行数据通信
#define      pm_Dout_GPIO_PORT                      GPIOE
#define      pm_Dout_GPIO_PIN                       GPIO_Pin_5

#define      pm_Dout_0                                GPIO_ResetBits ( pm_Dout_GPIO_PORT, pm_Dout_GPIO_PIN )
#define      pm_Dout_1                                GPIO_SetBits ( pm_Dout_GPIO_PORT, pm_Dout_GPIO_PIN )

#define      pm_Dout_IN()                              GPIO_ReadInputDataBit ( pm_Dout_GPIO_PORT, pm_Dout_GPIO_PIN )

void PM_Init(void);

uint8_t PM_Read_DATA(PM_Data_TypeDef *PM_Data);;

#endif

