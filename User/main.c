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
#include "sccb.h"
#include "ov2640.h"

void show_photo();
int main()
{
	float t = 0;
	usart_gpio_config(115200);
	dma_config();
	systick_config();
	LCD_Init();
	LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
	SCCB_OV2640_init();
	DCI_OV2640_Init();
	OV2640_ImageSize_Set(1000, 1000);
	OV2640_Outsize_Set(100, 100);
	nvic_irq_enable(DCI_IRQn, 0U, 0U);
	dci_interrupt_enable(DCI_INT_EF);
	dci_enable();
	while (1)
	{
		if (g_recv_complete_flag)
		{
			g_recv_complete_flag = 0;
			printf("字节长度:%d ", g_recv_length);
			printf("内容:%s\r\n", g_recv_buff);
			memset(g_recv_buff, 0, g_recv_length);
			g_recv_length = 0;
		}
		if (dci_flag_get(DCI_FLAG_OVR))
		{
			printf("溢出！\n");
		}
		delay_1ms(200);
		DCItoLCD_DMA_Init();
		dci_capture_enable();
		delay_1ms(150);
		dma_deinit(DMA_DCI,DMA_DCI_CH);
		dci_capture_disable();
		show_photo();
	}
}

void show_photo()
{
	int index = 0;
	for (uint16_t i = 0; i < 100; i++)
	{

		for (uint16_t j = 0; j < 50; j++)
		{
			uint32_t tmp = photo_buff[index];
			LCD_DrawPoint(j * 2, i, (uint16_t)(tmp & 0xFFFF));
			LCD_DrawPoint(j * 2 + 1, i, (uint16_t)(photo_buff[index] >> 16));
			index += 1;
		}
	}
}