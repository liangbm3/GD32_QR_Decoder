/************************************************
 * @file Name: bsp_usart.c
 * @brief : 串口配置源文件，采用DMA接收的方式
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-12
 *
 * modification history :
 * @date:       @version:      @author:
 * Changes:
 *************************************************/

#include "bsp_usart.h"
#include "stdio.h"
#include "bsp_dma.h"

uint8_t g_recv_buff[USART_RECEIVE_LENGTH]; // 定义接收用来接收数据的数组
uint16_t g_recv_length = 0;				   // 数组长度
uint8_t g_recv_complete_flag = 0;		   // 完成的标志位

/************************************************
 * @brief : 串口初始化函数
 * @param  band_rate      串口波特率
 * @date : 2024-05-12
 *************************************************/
void usart_gpio_config(uint32_t band_rate)
{
	/* 开启时钟 */
	rcu_periph_clock_enable(BSP_USART_TX_RCU);
	rcu_periph_clock_enable(BSP_USART_RX_RCU);
	rcu_periph_clock_enable(BSP_USART_RCU);

	/* 配置GPIO复用功能 */
	gpio_af_set(BSP_USART_TX_PORT, BSP_USART_AF, BSP_USART_TX_PIN);
	gpio_af_set(BSP_USART_RX_PORT, BSP_USART_AF, BSP_USART_RX_PIN);

	/* 配置GPIO的模式 */
	/* 配置TX为复用模式 上拉模式 */
	gpio_mode_set(BSP_USART_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BSP_USART_TX_PIN);
	/* 配置RX为复用模式 上拉模式 */
	gpio_mode_set(BSP_USART_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BSP_USART_RX_PIN);

	/* 配置TX为推挽输出 50MHZ */
	gpio_output_options_set(BSP_USART_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BSP_USART_TX_PIN);
	/* 配置RX为推挽输出 50MHZ */
	gpio_output_options_set(BSP_USART_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BSP_USART_RX_PIN);

	/* 配置串口的参数 */
	usart_deinit(BSP_USART);
	usart_baudrate_set(BSP_USART, band_rate);
	usart_parity_config(BSP_USART, USART_PM_NONE);
	usart_word_length_set(BSP_USART, USART_WL_8BIT);
	usart_stop_bit_set(BSP_USART, USART_STB_1BIT);

	/* 使能串口 */
	usart_enable(BSP_USART);
	usart_transmit_config(BSP_USART, USART_TRANSMIT_ENABLE);
	usart_receive_config(BSP_USART, USART_RECEIVE_ENABLE);

	nvic_irq_enable(BSP_USART_IRQ, 2, 2);
#if !USB_USART_DMA
	usart_interrupt_enable(BSP_USART, USART_INT_RBNE);
#endif
	usart_interrupt_enable(BSP_USART, USART_INT_IDLE);
}

/************************************************
 * @brief : 串口发送一个字节
 * @param  ucch    要发送的字节
 * @date : 2024-05-12
 *************************************************/
void usart_send_data(uint8_t ucch)
{
	usart_data_transmit(BSP_USART, (uint8_t)ucch);
	while (RESET == usart_flag_get(BSP_USART, USART_FLAG_TBE))
		;
}

/************************************************
 * @brief : 串口发送字符串
 * @param  ucstr     要发送的字符串
 * @date : 2024-05-12
 *************************************************/
void usart_send_string(uint8_t *ucstr)
{
	while (ucstr && *ucstr)
	{
		usart_send_data(*ucstr++);
	}
}

/************************************************
 * @brief : 重定义fputc
 * @param  ch
 * @param  f
 * @return int
 * @date : 2024-05-12
 *************************************************/
int fputc(int ch, FILE *f)
{
	usart_send_data(ch);
	// 等待发送数据缓冲区标志置位
	return ch;
}

/************************************************
 * @brief : 中断服务函数，串口空闲时进入中断
 * @date : 2024-05-12
*************************************************/
void USART0_IRQHandler(void)
{
#if !USB_USART_DMA
	if (usart_interrupt_flag_get(BSP_USART, USART_INT_FLAG_RBNE) == SET)
	{
		g_recv_buff[g_recv_length++] = usart_data_receive(BSP_USART);
	}
#endif

	if (usart_interrupt_flag_get(BSP_USART, USART_INT_FLAG_IDLE) == SET)//检测帧中断
	{
		usart_data_receive(BSP_USART);//必须要读，读出来的值不能要

#if USB_USART_DMA
		/*处理DMA接收到的数据*/
		g_recv_length = USART_RECEIVE_LENGTH - dma_transfer_number_get(BSP_DMA, BSP_DMA_CH);//时间接收的长度等于数组长度减去当前DMA剩余的数据量
		dma_channel_disable(BSP_DMA, BSP_DMA_CH);//失能DMA
		dma_config();//重新配置DMA
#endif

		g_recv_buff[g_recv_length] = '\0';
		g_recv_complete_flag = 1;
	}
}