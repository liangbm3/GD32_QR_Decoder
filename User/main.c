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
#include "led.h"
#include "bsp_usart.h"
#include "pic.h"
#include "lcdgui.h"
#include "lcdinit.h"
#include "ov2640.h"
#include "gd32f4xx_misc.h"

void DCI_IRQHandler(void)
{
	printf("666");
};

int main(void)
{
	float t = 0;
	systick_config();
	usart_gpio_config(115200);
	led_gpio_init();
	LCD_Init();
	uint8_t x=SCCB_OV2640_init();
	if(x==0)
	{
		printf("成功");
	}
	DCI_OV2640_Init();
	OV2640_OutSize_Set(480,280);
	nvic_irq_enable(DCI_IRQn,0U,0U);
	dci_interrupt_enable(DCI_INT_EF);
	dci_capture_enable();
	LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
	gpio_bit_write(PORT_LED2, PIN_LED2, SET);
	gpio_bit_write(PORT_LED1, PIN_LED1, SET);
	gpio_bit_write(PORT_LED3, PIN_LED3, SET);
	gpio_bit_write(PORT_LED4, PIN_LED4, SET);
	printf("lbm\n");
	while (1)
	{
		delay_1ms(200);
		gpio_bit_write(PORT_LED2, PIN_LED2, RESET);
		delay_1ms(200);
		gpio_bit_write(PORT_LED2, PIN_LED2, SET);
		LCD_ShowChinese(48, 0, "立创梁山派", RED, WHITE, 32, 0);

		LCD_ShowString(0, 40, "LCD_W:", RED, WHITE, 16, 0);
		LCD_ShowIntNum(48, 40, LCD_W, 3, RED, WHITE, 16);
		LCD_ShowString(80, 40, "LCD_H:", RED, WHITE, 16, 0);
		LCD_ShowIntNum(128, 40, LCD_H, 3, RED, WHITE, 16);

		LCD_ShowString(0, 70, "Increaseing Nun:", RED, WHITE, 16, 0);
		LCD_ShowFloatNum1(128, 70, t, 4, RED, WHITE, 16);
		t += 0.11;
		if (g_recv_complete_flag)
		{
			g_recv_complete_flag = 0;
			printf("g_recv_length:%d ", g_recv_length);
			printf("g_recv_buff:%s\r\n", g_recv_buff);
			memset(g_recv_buff, 0, g_recv_length);
			g_recv_length = 0;
		}
		if(dci_flag_get(DCI_FLAG_EF))
		{
			printf("有效");
		}
		else
		{
			printf("无效");
		}
		delay_1ms(1000);
	}
}


