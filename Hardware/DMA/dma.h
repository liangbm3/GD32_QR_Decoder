/************************************************
 * @file Name: dma.h
 * @brief : DMA宏文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-13
 *
 * modification history :
 * @date:       @version:      @author:
 * Changes:
 *************************************************/
#ifndef DMA_H
#define DMA_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_dma.h"

/* --------------------------------- DMA宏定义 --------------------------------- */
#define DMA_USART_RCU RCU_DMA1
#define DMA_USART DMA1
#define DMA_USART_CH DMA_CH2

#define DMA_DCI_RCU RCU_DMA1
#define DMA_DCI DMA1
#define DMA_DCI_CH  DMA_CH7

/* -------------------------------------------------------------------------- */

#define DCI_DR_ADDRESS     (0x50050028U)



void dma_config(void);
void DCItoLCD_DMA_Init(void);

#endif