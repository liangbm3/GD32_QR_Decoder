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
#define DMA_RCU   	       RCU_DMA1
#define DMA  				       DMA1
#define DMA_CH  		       DMA_CH2
/* -------------------------------------------------------------------------- */

void dma_config(void);
#endif