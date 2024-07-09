#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"

#define BSP_KEY1_RCU   RCU_GPIOD
#define BSP_KEY1_PORT  GPIOD
#define BSP_KEY1_PIN   GPIO_PIN_4

#define BSP_KEY2_RCU   RCU_GPIOD
#define BSP_KEY2_PORT  GPIOD
#define BSP_KEY2_PIN   GPIO_PIN_6


void key1_gpio_config(void);
uint8_t key1_scan(void);
void key2_gpio_config(void);
uint8_t key2_scan(void);

#endif