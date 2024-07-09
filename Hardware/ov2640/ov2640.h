/************************************************
 * @file Name: ov2640.h
 * @brief : OV2640头文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-09
 * 
 * modification history :
 * @date:       @version:      @author:     
 * Changes: 
*************************************************/
#ifndef OV2640_H
#define OV2640_H

#include "sccb.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_dci.h"
#include "systick.h"
#include <stdio.h>

extern int capture_flag;
/* -------------------------------------------------------------------------- */
//PIXCLK  PA6
//VSYNC   PG9
//HSYNC   PA4
//D0      PC6
//D1      PC7
//D2      PC8
//D3      PC9
//D4      PC11
//D5      PD3
//D6      PB8
//D7      PB9
//RST     PA12
//PWDN    PA2
/* -------------------------------------------------------------------------- */

//RST端口宏定义
#define RCU_RST RCU_GPIOA
#define PORT_RST GPIOA
#define GPIO_RST GPIO_PIN_12

//PWDN端口宏定义
#define RCU_PWDN RCU_GPIOA
#define PORT_PWDN GPIOA
#define GPIO_PWDN GPIO_PIN_2

typedef struct
{
  uint8_t manufacturer_id1;  // Manufacturer ID Byte - High  (Read only = 0x7F)
  uint8_t manufacturer_id2;  // Manufacturer ID Byte - Low   (Read only = 0xA2)
  uint8_t version;           // Product ID Number LSB
  uint8_t pid; 							 // Product ID Number MSB
}ov2640_id_struct; 

#define OV2640_PWDN_OUT(x)     gpio_bit_write(PORT_PWDN, GPIO_PWDN, x?SET:RESET)
#define OV2640_RST_OUT(x)      gpio_bit_write(PORT_RST,  GPIO_RST,  x?SET:RESET)

#define OV2640_VER        0x0B
#define OV2640_MIDH       0x1C 
#define OV2640_MIDL       0x1D 



void DCI_OV2640_Init(void);

uint8_t SCCB_OV2640_init(void);
uint8_t OV2640_Outsize_Set(uint16_t width, uint16_t height);
uint8_t dci_ov2640_id_read(ov2640_id_struct *ov2640id);
uint8_t OV2640_ImageSize_Set(uint16_t width, uint16_t height);

#endif