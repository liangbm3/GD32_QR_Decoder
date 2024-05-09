/************************************************
 * @file Name: bsp_led.h
 * @brief : 
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-09
 * 
 * modification history :
 * @date:       @version:      @author:     
 * Changes: 
*************************************************/
#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"

#define RCU_LED2  	RCU_GPIOD   // GPIOD的时钟
#define PORT_LED2 	GPIOD				// GPIOD的端口
#define PIN_LED2 		GPIO_PIN_7  // GPIOD的引脚

void led_gpio_config(void); 	  // led gpio引脚配置

#endif  /* BSP_LED_H */
