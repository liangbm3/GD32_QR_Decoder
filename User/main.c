/************************************************
 * @file Name: main.c
 * @brief :
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-09
 *
 * modification history :
 * @date:       @version:      @author:
 * Changes:
 *************************************************/
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "main.h"
#include "dma.h"
#include "usart.h"

int main()
{
	usart_gpio_config(115200);
	dma_config();
	systick_config();
	while (1)
	{
		delay_1ms(1000);
		printf("你好，世界！\n");
		if (g_recv_complete_flag)
		{
			g_recv_complete_flag = 0;
			printf("字节长度:%d ", g_recv_length);
			printf("内容:%s\r\n", g_recv_buff);
			memset(g_recv_buff, 0, g_recv_length);
			g_recv_length = 0;
		}
	}
}
