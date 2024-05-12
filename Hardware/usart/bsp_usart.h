/************************************************
 * @file Name: bsp_usart.h
 * @brief : 涓插彛鎺ユ敹閰嶇疆澶存枃浠�
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-12
 * 
 * modification history :
 * @date:       @version:      @author:     
 * Changes: 
*************************************************/

#ifndef _BSP_USART_H
#define _BSP_USART_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_dma.h"
#include "gd32f4xx_usart.h"
#include "gd32f4xx_gpio.h"

#define BSP_USART_TX_RCU  RCU_GPIOA
#define BSP_USART_RX_RCU  RCU_GPIOA
#define BSP_USART_RCU     RCU_USART0

#define BSP_USART_TX_PORT GPIOA
#define BSP_USART_RX_PORT GPIOA
#define BSP_USART_AF 			GPIO_AF_7
#define BSP_USART_TX_PIN  GPIO_PIN_9
#define BSP_USART_RX_PIN  GPIO_PIN_10

#define BSP_USART 				USART0
#define BSP_USART_IRQ     USART0_IRQn

#define USART_RECEIVE_LENGTH  4096


/*0：中断接收；1：DMA接收*/
#define USB_USART_DMA 1

extern uint8_t g_recv_buff[USART_RECEIVE_LENGTH];
extern uint16_t g_recv_length;
extern uint8_t g_recv_complete_flag; 


void usart_gpio_config(uint32_t band_rate);
void usart_send_data(uint8_t ucch);
void usart_send_string(uint8_t *ucstr);

#endif