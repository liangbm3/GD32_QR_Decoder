/************************************************
 * @file Name: usart.c
 * @brief :usart源文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-13
 *
 * modification history :
 * @date:       @version:      @author:
 * Changes:
 *************************************************/

#include "usart.h"
#include <stdio.h>
#include "dma.h"
uint8_t g_recv_buff[USART_RECEIVE_LENGTH]; // 定义接收用来接收数据的数组
uint16_t g_recv_length = 0;                // 数组长度
uint8_t g_recv_complete_flag = 0;          // 完成的标志位

uint32_t photo_buff[10000];
uint16_t photo_length;
uint8_t photo_complete_flag;

/************************************************
 * @brief : usart初始化函数，对串口进行初始化
 * @param  band_rate        
 * @date : 2024-05-13
*************************************************/
void usart_gpio_config(uint32_t band_rate)
{
    /* 开启时钟 */
    rcu_periph_clock_enable(USART_TX_RCU);
    rcu_periph_clock_enable(USART_RX_RCU);
    rcu_periph_clock_enable(USART_RCU);

    /* 配置GPIO复用功能 */
    gpio_af_set(USART_TX_PORT, USART_AF, USART_TX_PIN);
    gpio_af_set(USART_RX_PORT, USART_AF, USART_RX_PIN);

    /* 配置GPIO的模式 */
    /* 配置TX为复用模式 上拉模式 */
    gpio_mode_set(USART_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_TX_PIN);
    /* 配置RX为复用模式 上拉模式 */
    gpio_mode_set(USART_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_RX_PIN);

    /* 配置TX为推挽输出 50MHZ */
    gpio_output_options_set(USART_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART_TX_PIN);
    /* 配置RX为推挽输出 50MHZ */
    gpio_output_options_set(USART_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART_RX_PIN);

    /* 配置串口的参数 */
    usart_deinit(USART);                         // 复位串口
    usart_baudrate_set(USART, band_rate);        // 设置波特率
    usart_parity_config(USART, USART_PM_NONE);   // 无校验位
    usart_word_length_set(USART, USART_WL_8BIT); // 8位数据位
    usart_stop_bit_set(USART, USART_STB_1BIT);   // 1位停止位

    /* 使能串口 */
    usart_enable(USART);
    usart_transmit_config(USART, USART_TRANSMIT_ENABLE); // 使能发送
    usart_receive_config(USART, USART_RECEIVE_ENABLE);   // 使能接收

    nvic_irq_enable(USART0_IRQn, 2, 2);            // 配置中断优先级
    usart_interrupt_enable(USART, USART_INT_IDLE); // 空闲检测中断，在一帧数据传输完成后触发中断
}

/************************************************
 * @brief : 串口发送一个字节
 * @param  ucch    要发送的字节
 * @date : 2024-05-12
 *************************************************/
void usart_send_data(uint8_t ucch)
{
    usart_data_transmit(USART, (uint8_t)ucch);
    while (RESET == usart_flag_get(USART, USART_FLAG_TBE))
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
 * @brief : 串口空闲中断函数，对DMA进行重新配置
 * @date : 2024-05-13
*************************************************/
void USART0_IRQHandler(void)
{
    if (usart_interrupt_flag_get(USART, USART_INT_FLAG_IDLE) == SET) // 检测帧中断
    {
        usart_data_receive(USART); // 必须要读，读出来的值不能要

        /*处理DMA接收到的数据*/
        g_recv_length = USART_RECEIVE_LENGTH - dma_transfer_number_get(DMA_USART, DMA_USART_CH); // 时间接收的长度等于数组长度减去当前DMA剩余的数据量
        dma_channel_disable(DMA_USART, DMA_USART_CH);                                            // 失能DMA
        dma_config();                                                                        // 重新配置DMA
        g_recv_buff[g_recv_length] = '\0';
        g_recv_complete_flag = 1;
    }
}