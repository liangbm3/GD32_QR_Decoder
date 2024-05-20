/************************************************
 * @file Name: sccb.h
 * @brief : 
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-16
 * 
 * modification history :
 * @date:       @version:      @author:     
 * Changes: 
*************************************************/

#ifndef SCCB_H
#define SCCB_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_i2c.h"

/* -------------------------------------------------------------------------- */
//SDA   PB7
//SCL   PB6
/* -------------------------------------------------------------------------- */
#define RCU_OV2640_SDA      RCU_GPIOB
#define PORT_OV2640_SDA     GPIOB
#define GPIO_OV2640_SDA     GPIO_PIN_7

#define RCU_OV2640_SCL      RCU_GPIOB
#define PORT_OV2640_SCL     GPIOB
#define GPIO_OV2640_SCL     GPIO_PIN_6

//SDA输出模式
#define OV2640_SDA_MODE_OUT()   gpio_mode_set(PORT_OV2640_SDA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_OV2640_SDA)
#define OV2640_SDA_MODE_IN()    gpio_mode_set(PORT_OV2640_SDA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_OV2640_SDA)

//获取SDA引脚变化
#define SDA_GET()	gpio_input_bit_get(PORT_OV2640_SDA,GPIO_OV2640_SDA)

//SDA和SCL输出
#define SDA(x)          gpio_bit_write(PORT_OV2640_SDA,GPIO_OV2640_SDA, (x?SET:RESET))
#define SCL(x)          gpio_bit_write(PORT_OV2640_SCL,GPIO_OV2640_SCL, (x?SET:RESET))

#define IIC_ADDR   			0X60  			//OV2640ID 

void OV2640_IIC_Init(void);
uint8_t IIC_RD_Reg(uint8_t reg);
uint8_t IIC_WR_Reg(uint8_t reg,uint8_t data);

#endif