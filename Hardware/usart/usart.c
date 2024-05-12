/************************************************
 * @file Name: usart.c
 * @brief : 串口源文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-09
 *
 * modification history :
 * @date:       @version:      @author:
 * Changes:
 *************************************************/
#include "usart.h"
#include "bsp_dma.h"
#include <stdio.h>
/************************************************
 * @brief : 串口初始化
 * @param  rate      串口波特率
 * @date : 2024-05-09
 *************************************************/

uint8_t g_recv_buff[USART_RECEIVE_LENGTH];
uint16_t g_recv_length = 0;
uint8_t g_recv_complete_flag = 0; 
void usart_init(uint32_t rate)
{
    // 开启时钟
    rcu_periph_clock_enable(USART_RCU);    // 开启串口时钟
    rcu_periph_clock_enable(USART_TX_RCU); // 开启端口时钟
    rcu_periph_clock_enable(USART_RX_RCU); // 开启端口时钟

    // 配置复用功能
    gpio_af_set(USART_TX_PORT, USART_AF, USART_TX_PIN);
    gpio_af_set(USART_RX_PORT, USART_AF, USART_RX_PIN);

    // 配置GPIO模式
    gpio_mode_set(USART_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_TX_PIN);
    gpio_mode_set(USART_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_RX_PIN);

    // 配置GPIO输出
    gpio_output_options_set(USART_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART_TX_PIN);
    gpio_output_options_set(USART_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART_RX_PIN);

    /* 串口配置*/
    usart_deinit(USART0);                         // 复位串口
    usart_baudrate_set(USART0, rate);             // 设置波特率
    usart_parity_config(USART0, USART_PM_NONE);   // 没有校验位
    usart_word_length_set(USART0, USART_WL_8BIT); // 8位数据位
    usart_stop_bit_set(USART0, USART_STB_1BIT);   // 1位停止位

    // 使能串口
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE); // 使能串口发送
    usart_transmit_config(USART0, USART_RECEIVE_ENABLE);  // 使能串口接收
    usart_enable(USART0);                                 // 使能串口

    nvic_irq_enable(USART0_IRQn, 2, 2);
    usart_interrupt_enable(USART0, USART_INT_IDLE);
}

/************************************************
 * @brief : 串口发送一个字节
 * @param  ucch     要发送的字节
 * @date : 2024-05-09
 *************************************************/
void usart_send_data(uint8_t ucch)
{
    usart_data_transmit(USART0, (uint8_t)ucch);
    while (RESET == usart_flag_get(USART0, USART_FLAG_TBE))
        ; // 等待发送数据缓冲区标志置位
}

/************************************************
 * @brief : 串口发送字符串
 * @param  ucstr     字符串首地址
 * @date : 2024-05-09
 *************************************************/
void usart_send_String(uint8_t *ucstr)
{
    while (ucstr && *ucstr) // 地址为空或者值为空跳出
    {
        usart_send_data(*ucstr++);
    }
}

/************************************************
 * @brief : 串口重定向
 * @param  ch
 * @param  f
 * @return int
 * @date : 2024-05-09
 *************************************************/
int fputc(int ch, FILE *f)
{
    usart_send_data(ch);
    // 等待发送数据缓冲去标志置位
    return ch;
}

void USART0_IRQHandler(void)
{

    if (usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) == SET)
    {
        usart_data_receive(USART0);

        g_recv_length = USART_RECEIVE_LENGTH - dma_transfer_number_get(BSP_DMA, BSP_DMA_CH);
        dma_channel_disable(BSP_DMA, BSP_DMA_CH);
        dma_config();
        g_recv_buff[g_recv_length] = '\0';
        g_recv_complete_flag = 1;
    }
}