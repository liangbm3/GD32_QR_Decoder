/************************************************
 * @file Name: lcd_init.h
 * @brief : lcd初始化头文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-15
 *
 * modification history :
 * @date:       @version:      @author:
 * Changes:
 *************************************************/
#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "systick.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_spi.h"
#define delay_ms delay_1ms

#ifndef u8
#define u8 uint8_t
#endif

#ifndef u16
#define u16 uint16_t
#endif

#ifndef u32
#define u32 uint32_t
#endif

/* --------------------------------- LCD端口移植 -------------------------------- */
// VCC - 3.3V
// SCL - PB13 SPI1_SCK
// SDA - PB15 SPI1_MOSI
// RES - PD0(可以接入复位)
// DC  - PG6
// CS  - PB12
// BLK - PG7

#define RCU_LCD_SCL RCU_GPIOB // SCK
#define PORT_LCD_SCL GPIOB
#define GPIO_LCD_SCL GPIO_PIN_13

#define RCU_LCD_SDA RCU_GPIOB // MOSI
#define PORT_LCD_SDA GPIOB
#define GPIO_LCD_SDA GPIO_PIN_15

#define RCU_LCD_CS RCU_GPIOB // NSS
#define PORT_LCD_CS GPIOB
#define GPIO_LCD_CS GPIO_PIN_12

#define RCU_LCD_DC RCU_GPIOG // DC
#define PORT_LCD_DC GPIOG
#define GPIO_LCD_DC GPIO_PIN_6

#define RCU_LCD_RES RCU_GPIOD // RES
#define PORT_LCD_RES GPIOD
#define GPIO_LCD_RES GPIO_PIN_0

#define RCU_LCD_BLK RCU_GPIOG // BLK
#define PORT_LCD_BLK GPIOG
#define GPIO_LCD_BLK GPIO_PIN_7

#define RCU_SPI_HARDWARE RCU_SPI1
#define PORT_SPI SPI1
#define LINE_AF_SPI GPIO_AF_5

/* -------------------------------------------------------------------------- */

#define USE_HORIZONTAL 1 // 设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#define USE_HARDWARE 1 // 是否使用硬件SPI， 1使用硬件SPI；0使用软件SPI

#if USE_HORIZONTAL == 0 || USE_HORIZONTAL == 1
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif

//-----------------LCD端口定义----------------

#define LCD_SCLK_Clr() gpio_bit_write(PORT_LCD_SCL, GPIO_LCD_SCL, RESET) // SCL=SCLK
#define LCD_SCLK_Set() gpio_bit_write(PORT_LCD_SCL, GPIO_LCD_SCL, SET)

#define LCD_MOSI_Clr() gpio_bit_write(PORT_LCD_SDA, GPIO_LCD_SDA, RESET) // SDA=MOSI
#define LCD_MOSI_Set() gpio_bit_write(PORT_LCD_SDA, GPIO_LCD_SDA, SET)

#define LCD_RES_Clr() gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, RESET) // RES
#define LCD_RES_Set() gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET)

#define LCD_DC_Clr() gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, RESET) // DC
#define LCD_DC_Set() gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET)

#define LCD_CS_Clr() gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, RESET) // CS
#define LCD_CS_Set() gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET)

#define LCD_BLK_Clr() gpio_bit_write(PORT_LCD_BLK, GPIO_LCD_BLK, RESET) // BLK
#define LCD_BLK_Set() gpio_bit_write(PORT_LCD_BLK, GPIO_LCD_BLK, SET)

void LCD_GPIO_Init(void);                             // 初始化GPIO
void LCD_Writ_Bus(u8 dat);                            // 模拟SPI时序
void LCD_WR_DATA8(u8 dat);                            // 写入一个字节
void LCD_WR_DATA(u16 dat);                            // 写入两个字节
void LCD_WR_REG(u8 dat);                              // 写入一个指令
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2); // 设置坐标函数
void LCD_Init(void);                                  // LCD初始化
#endif
