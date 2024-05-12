/************************************************
 * @file Name: led.h
 * @brief : 板载LED灯头文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-09
 * 
 * modification history :
 * @date:       @version:      @author:     
 * Changes: 
*************************************************/
#ifndef LED_H
#define LED_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"

//LED1
#define RCU_LED1    RCU_GPIOE
#define PORT_LED1   GPIOE
#define PIN_LED1    GPIO_PIN_3

//LED2
#define RCU_LED2    RCU_GPIOD
#define PORT_LED2   GPIOD
#define PIN_LED2    GPIO_PIN_7

//LED3
#define RCU_LED3    RCU_GPIOG
#define PORT_LED3   GPIOG
#define PIN_LED3    GPIO_PIN_3

//LED4
#define RCU_LED4    RCU_GPIOA
#define PORT_LED4   GPIOA
#define PIN_LED4    GPIO_PIN_5


/* -------------------------------------------------------------------------- */
void led1_gpio_init(void);
void led2_gpio_init(void);
void led3_gpio_init(void);
void led4_gpio_init(void);
void led_gpio_init(void);
#endif  
