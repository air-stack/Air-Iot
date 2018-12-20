
#include "bsp_gp2y.h"
#include "bsp_SysTick.h"


static void PM_GPIO_Config(void);

static void PM_Mode_IPU(void);

static void PM_Mode_Out_PP(void);

static uint8_t PM_ReadByte(void);

/**
 * @brief  PM25 初始化函数
 * @param  无
 * @retval 无
 */
void PM_Init(void) {
    PM_GPIO_Config();

    pm_Dout_1;               // 拉高GPIOB10
}

/*
 * 函数名：PM25_GPIO_Config
 * 描述  ：配置PM25用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
static void PM_GPIO_Config(void) {
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;


    /*开启macPM25_Dout_GPIO_PORT的外设时钟*/
    pm_Dout_SCK_APBxClock_FUN(pm_Dout_GPIO_CLK, ENABLE);

    /*选择要控制的macPM25_Dout_GPIO_PORT引脚*/
    GPIO_InitStructure.GPIO_Pin = pm_Dout_GPIO_PIN;

    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*调用库函数，初始化macPM25_Dout_GPIO_PORT*/
    GPIO_Init(pm_Dout_GPIO_PORT, &GPIO_InitStructure);

}

/*
 * 函数名：PM25_Mode_IPU
 * 描述  ：使PM25-DATA引脚变为上拉输入模式
 * 输入  ：无
 * 输出  ：无
 */
static void PM_Mode_IPU(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    /*选择要控制的macPM25_Dout_GPIO_PORT引脚*/
    GPIO_InitStructure.GPIO_Pin = pm_Dout_GPIO_PIN;

    /*设置引脚模式为浮空输入模式*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

    /*调用库函数，初始化macPM25_Dout_GPIO_PORT*/
    GPIO_Init(pm_Dout_GPIO_PORT, &GPIO_InitStructure);

}

/*
 * 函数名：PM25_Mode_Out_PP
 * 描述  ：使PM25-DATA引脚变为推挽输出模式
 * 输入  ：无
 * 输出  ：无
 */
static void PM_Mode_Out_PP(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    /*选择要控制的macPM25_Dout_GPIO_PORT引脚*/
    GPIO_InitStructure.GPIO_Pin = pm_Dout_GPIO_PIN;

    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*调用库函数，初始化macPM25_Dout_GPIO_PORT*/
    GPIO_Init(pm_Dout_GPIO_PORT, &GPIO_InitStructure);

}


/*
 * 从PM25读取一个字节，MSB先行
 */
static uint8_t PM_ReadByte(void) {
    uint8_t i, temp = 0;

    // 读取8位一个字节的数据
    for (i = 0; i < 8; i++) {
        /*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/
        while (pm_Dout_IN() == Bit_RESET);

        /*PM25 以26~28us的高电平表示“0”，以70us高电平表示“1”，
         *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时
         */
        // FIXME 根据传感器比特率，修改延时 TODO 400
        Delay_us(40); //延时x us 这个延时需要大于数据0持续的时间即可

        if (pm_Dout_IN() == Bit_SET)/* x us后仍为高电平表示数据“1” */
        {
            /* 等待数据1的高电平结束 */
            while (pm_Dout_IN() == Bit_SET);

            temp |= (uint8_t)(0x01 << (7 - i));  //把第7-i位置1，MSB先行
        } else     // x us后为低电平表示数据“0”
        {
            temp &= (uint8_t)
            ~(0x01 << (7 - i)); //把第7-i位置0，MSB先行
        }
    }

    return temp;

}

/*
 * 一次完整的数据传输为7个字节，高位先出，数据格式请参考GP2Y传感器说明书
 */
uint8_t PM_Read_DATA(PM_Data_TypeDef *PM_Data) {
    /*输出模式*/
    PM_Mode_Out_PP();
    /*主机拉低*/
    pm_Dout_0;
    /*延时18ms*/
    Delay_ms(18);

    /*总线拉高 主机延时30us*/
    pm_Dout_1;

    Delay_us(30);   //延时30us

    /*主机设为输入 判断从机响应信号*/
    PM_Mode_IPU();

    /*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/
    if (pm_Dout_IN() == Bit_RESET) {
        /*轮询直到从机发出 的80us 低电平 响应信号结束*/
        while (pm_Dout_IN() == Bit_RESET);

        /*轮询直到从机发出的 80us 高电平 标置信号结束*/
        while (pm_Dout_IN() == Bit_SET);

        /* 开始接收数据 */
        PM_Data->start = PM_ReadByte();

        PM_Data->voutH = PM_ReadByte();

        PM_Data->voutL = PM_ReadByte();

        PM_Data->vrefH = PM_ReadByte();

        PM_Data->vrefL = PM_ReadByte();

        PM_Data->check_sum = PM_ReadByte();

        PM_Data->finish = PM_ReadByte();


        /*读取结束，引脚改为输出模式*/
        PM_Mode_Out_PP();
        /*主机拉高*/
        pm_Dout_1;

        return SUCCESS;
    } else
        return ERROR;

}

