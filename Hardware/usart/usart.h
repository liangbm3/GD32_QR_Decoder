/************************************************
 * @file Name: usart.h
 * @brief : 串口头文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-09
 * 
 * modification history :
 * @date:       @version:      @author:     
 * Changes: 
*************************************************/
#ifndef USART_H
#define USART_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_usart.h"
#include <stdio.h>
/* -------------------------------------------------------------------------- */
//使用串口0 
//TX    PA9
//RX    PA10
/* -------------------------------------------------------------------------- */

//RCU
#define USART_RCU RCU_USART0
#define USART_TX_RCU RCU_GPIOA
#define USART_RX_RCU RCU_GPIOA

//PORT
#define USART_TX_PORT GPIOA
#define USART_RX_PORT GPIOA

//PIN
#define USART_TX_PIN GPIO_PIN_9
#define USART_RX_PIN GPIO_PIN_10

//复用模式
#define USART_AF GPIO_AF_7

#define USART_RECEIVE_LENGTH  4096

/* -------------------------------------------------------------------------- */
void usart_init(uint32_t rate);
void usart_send_data(uint8_t ucch);
void usart_send_String(uint8_t *ucstr);

extern uint8_t g_recv_buff[USART_RECEIVE_LENGTH];
extern uint16_t g_recv_length;
extern uint8_t g_recv_complete_flag;

#endif

