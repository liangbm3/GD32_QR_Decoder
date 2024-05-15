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
#include "lcd.h"
#include "lcd_init.h"
#include "pic.h"

int main()
{
	float t = 0;
	usart_gpio_config(115200);
	printf("ok\n");
	dma_config();
	systick_config();
	LCD_Init();
	if(RESET == spi_i2s_flag_get(PORT_SPI, SPI_FLAG_TBE))
	{
		printf("yes\n");
	}
	LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
		if(RESET == spi_i2s_flag_get(PORT_SPI, SPI_FLAG_TBE))
	{
		printf("yes\n");
	}
	while (1)
	{
		delay_1ms(100);
		LCD_ShowString(0, 0, "QQ", RED, WHITE, 24, 0);
		LCD_ShowString(24, 30, "LCD_W:", RED, WHITE, 16, 0);
		LCD_ShowIntNum(72, 30, LCD_W, 3, RED, WHITE, 16);
		LCD_ShowString(24, 50, "LCD_H:", RED, WHITE, 16, 0);
		LCD_ShowIntNum(72, 50, LCD_H, 3, RED, WHITE, 16);
		LCD_ShowFloatNum1(20, 80, t, 4, RED, WHITE, 16);
		t += 0.11;
		LCD_ShowPicture(65, 80, 40, 40, gImage_1);
				if(SET == spi_i2s_flag_get(PORT_SPI, SPI_FLAG_TBE))
	{
		printf("no\n");
	}
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
