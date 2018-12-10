#include "wifi_function.h"
#include "wifi_config.h"
#include "bsp_gpio.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "bsp_SysTick.h"
#include <string.h> 


/*
 * 函数名：hlk_rm04_check_cmd
 * 描述  ：向WF-HLK-RM04发送指令后,检测接收到的应答
 * 输入  ：str，期待的应答内容
 * 返回  : 0，应答失败
 *         其他，应答成功
 * 调用  ：被hlk_rm04_send_cmd调用
 */
u8 * hlk_rm04_check_cmd( char * str )
{
	char * strx = 0;
	
	if( strUSART2_Fram_Record .InfBit .FramFinishFlag )		                                       //已接收到一帧数据
	strx = strstr( ( const char * ) strUSART2_Fram_Record .Data_RX_BUF, ( const char * ) str ); 
	
	return ( u8 * ) strx;
	
}


/*
 * 函数名：hlk_rm04_send_cmd
 * 描述  ：向WF-HLK-RM04发送指令
 * 输入  ：cmd，待发送的指令
           ack，期待的应答内容
           waittime，期待应答的超时时间（ms）
 * 返回  : 0，指令发送失败
 *         1，指令发送成功
 * 调用  ：被hlk_rm04_quit_trans，hlk_rm04_net_config调用
 */
u8 hlk_rm04_send_cmd( char * cmd, char * ack, u16 waittime )
{
	u8 ReturnValue = 1; 
	
	strUSART2_Fram_Record .InfAll = 0;

	USART2_printf( USART2, "%s\r", cmd );                          	                             //发送命令
	
	if( ack && waittime )		                                                                     //需要等待应答
	{
		while( -- waittime )	                                                                     //等待倒计时
		{
			Delay_ms( 1 );
			
			if( strUSART2_Fram_Record .InfBit .FramFinishFlag )                                      //接收到期待的应答结果
			{
				if( hlk_rm04_check_cmd( ack ) ) break;                                                 //得到有效数据 
				strUSART2_Fram_Record .InfBit .FramFinishFlag = 0;
			} 
		} 
		
		if( waittime == 0 ) ReturnValue = 0; 
		
	}
	
	return ReturnValue;
	
} 


/*
 * 函数名hlk_rm04_quit_trans
 * 描述  ：让WF-HLK-RM04退出透传模式
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
void hlk_rm04_quit_trans( void )
{
	do{
  ES_LOW_LEVEL();
	Delay_ms( 120 );
	ES_HIGH_LEVEL();
	} while( ! hlk_rm04_send_cmd( "at", "\r\n", 1000 ) );                                        //判断是否已退出透传模式

}


/*
 * 函数名hlk_rm04_net_config
 * 描述  ：配置WF-HLK-RM04模块
 * 输入  ：p，配置WF-HLK-RM04模块的指令集
 * 返回  : 无
 * 调用  ：被外部调用
 */
 void hlk_rm04_net_config( STRUCT_Net_ConfigTypeDef * p )
{
	hlk_rm04_send_cmd( p -> netmode, "ok", 500 );

	if ( ! strstr( p -> netmode, "1" ) )
	{
		hlk_rm04_send_cmd( p -> wifi_conf, "ok", 500 ); 

		if ( strstr( p -> netmode, "3" ) )
		{
			hlk_rm04_send_cmd( p -> dhcpd, "ok", 500 );
			
			hlk_rm04_send_cmd( p -> dhcpd_ip, "ok", 500 );
			
			hlk_rm04_send_cmd( p -> dhcpd_dns, "ok", 500 );	
			
		}
		
	}	

  if ( strstr( p -> netmode, "3" ) )	
	{
		hlk_rm04_send_cmd( p -> net_ip, "ok", 500 );
		
		hlk_rm04_send_cmd( p -> net_dns, "ok", 500 );		
		
	}	
	else
	{
		hlk_rm04_send_cmd( p -> dhcpc, "ok", 500 );
		
		if ( strstr( p -> dhcpc, "0" ) )	
		{
			hlk_rm04_send_cmd( p -> net_ip, "ok", 500 );
			
			hlk_rm04_send_cmd( p -> net_dns, "ok", 500 );		
			
		}
	
	}
	
	hlk_rm04_send_cmd( p -> remotepro, "ok", 500 );
	
	hlk_rm04_send_cmd( p -> mode, "ok", 500 );
	
  hlk_rm04_send_cmd( p -> remoteport, "ok", 500 );
	
  if ( strstr( p -> mode, "client" ) )	
	{
		hlk_rm04_send_cmd( p -> remoteip, "ok", 500 ); 
		
		hlk_rm04_send_cmd( p -> CLport, "ok", 500 );		
		
	}	
	
	hlk_rm04_send_cmd( p -> timeout, "ok", 500 );
	
	hlk_rm04_send_cmd( p -> uart, "ok", 500 );
	
	hlk_rm04_send_cmd( p -> uartpacklen, "ok", 500 );
	
	hlk_rm04_send_cmd( p -> uartpacktimeout, "ok", 500 );
	
	if ( strstr( p -> netmode, "3" ) )
	hlk_rm04_send_cmd( "at+net_commit=1", "\r\n\r\n", 20000 );
	else 
	hlk_rm04_send_cmd( "at+net_commit=1", "ok", 20000 );
	
	hlk_rm04_send_cmd( "at+net_wanip=?", ".", 500 );
	
	hlk_rm04_send_cmd( "at+reconn=1", "ok", 500 );	
	
}


