/************************************************
 * @file Name: usart.h
 * @brief : usart宏文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-13
 * 
 * modification history :
 * @date:       @version:      @author:     
 * Changes: 
*************************************************/

#ifndef USART_H
#define USART_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_dma.h"
#include "gd32f4xx_usart.h"
#include "gd32f4xx_gpio.h"

/* ---------------------------------- 引脚宏定义 --------------------------------- */
#define USART_TX_RCU  RCU_GPIOA
#define USART_RX_RCU  RCU_GPIOA
#define USART_TX_PORT GPIOA
#define USART_RX_PORT GPIOA
#define USART_TX_PIN  GPIO_PIN_9
#define USART_RX_PIN  GPIO_PIN_10
#define USART_RCU     RCU_USART0
/* -------------------------------------------------------------------------- */

/* --------------------------------- 串口相关宏定义 -------------------------------- */
#define USART_AF 			GPIO_AF_7
#define USART 				USART0
/* -------------------------------------------------------------------------- */




#define USART_RECEIVE_LENGTH  4096



extern uint8_t g_recv_buff[USART_RECEIVE_LENGTH];
extern uint16_t g_recv_length;
extern uint8_t g_recv_complete_flag; 

extern uint32_t photo_buff[10000];
extern uint16_t photo_length;
extern uint8_t photo_complete_flag;

void usart_gpio_config(uint32_t band_rate);
void usart_send_data(uint8_t ucch);
void usart_send_string(uint8_t *ucstr);

#endif