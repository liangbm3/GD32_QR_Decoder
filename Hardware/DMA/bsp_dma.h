/************************************************
 * @file Name: bsp_dma.h
 * @brief : DMA配置头文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-12
 * 
 * modification history :
 * @date:       @version:      @author:     
 * Changes: 
*************************************************/
#ifndef _BSP_DMA_H
#define _BSP_DMA_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_dma.h"
#define BSP_DMA_RCU   	       RCU_DMA1
#define BSP_DMA  				       DMA1
#define BSP_DMA_CH  		       DMA_CH2
#define BSP_DMA_CH_IRQ  			 DMA1_Channel2_IRQn
#define BSP_DMA_CH_IRQ_HANDLER DMA1_Channel2_IRQHandler

void dma_config(void);
#endif