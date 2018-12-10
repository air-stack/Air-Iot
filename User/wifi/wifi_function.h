#ifndef __WIFI_FUNCTION_H
#define	__WIFI_FUNCTION_H 


#include "stm32f10x.h"
#include "wifi_config.h"


#define ES_HIGH_LEVEL()          GPIO_SetBits( GPIOC, GPIO_Pin_5 )
#define ES_LOW_LEVEL()           GPIO_ResetBits( GPIOC, GPIO_Pin_5 )


void hlk_rm04_quit_trans( void );
void hlk_rm04_net_config( STRUCT_Net_ConfigTypeDef * mode );
static u8 hlk_rm04_send_cmd( char *cmd, char *ack, u16 waittime );
static u8* hlk_rm04_check_cmd( char *str );


#endif    /* __WIFI_FUNCTION_H */





