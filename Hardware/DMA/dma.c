/************************************************
 * @file Name: dma.c
 * @brief : DMA源文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-13
 * 
 * modification history :
 * @date:       @version:      @author:     
 * Changes: 
*************************************************/
#include "gd32f4xx.h"
#include "dma.h"
#include "usart.h"

/************************************************
 * @brief : DMA配置函数，对DMA进行初始化
 * @date : 2024-05-13
*************************************************/
void dma_config(void)
{
	dma_single_data_parameter_struct dma_init_struct;//定义DMA参数结构体
	rcu_periph_clock_enable(DMA_USART_RCU);//开启DMA时钟
	dma_deinit(DMA_USART, DMA_USART_CH);//复位DMA

	/*DMA参数结构体的配置*/
	dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);//外设基地址，取USART0的地址
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;//外设地址生成算法，串口地址不变，设置为固定模式
	dma_init_struct.memory0_addr = (uint32_t)g_recv_buff;//存储器基地址，定义的数组的地址
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;//存储器基地址，每次存放的地址不同，要配置为增量模式
	dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;//外设和存储器数据传输宽度，设置为8位长度
	dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;//DMA循环模式，关闭循环模式
	dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;//DMA数据传输方向，外设到存储器
	dma_init_struct.number = USART_RECEIVE_LENGTH;//DMA通道数据传输数量，数据量超过这个值时，超出的部分不会被接收到
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;//DMA的软件优先级，这里配置为超高优先级

	/*定义单一传输参数结构体*/
	dma_single_data_mode_init(DMA_USART, DMA_USART_CH, &dma_init_struct);
	/*使能通道外设*/
	dma_channel_subperipheral_select(DMA_USART, DMA_USART_CH, DMA_SUBPERI4);
	/*使能DMA通道*/
	dma_channel_enable(DMA_USART, DMA_USART_CH);
	/*使能DMA中断*/
	dma_interrupt_enable(DMA_USART, DMA_USART_CH, DMA_CHXCTL_FTFIE);//通道传输完成中断
	nvic_irq_enable(DMA1_Channel2_IRQn, 2, 1);//配置中断优先级
	/*使能串口接收DMA*/
	usart_dma_receive_config(USART0, USART_DENR_ENABLE);
}

/************************************************
 * @brief : DMA传输完成中断函数，清除中断完成标志位
 * @date : 2024-05-13
*************************************************/
void DMA1_Channel2_IRQHandler(void)
{
	if (dma_interrupt_flag_get(DMA_USART, DMA_USART_CH, DMA_INT_FLAG_FTF) == SET)
	{
		dma_interrupt_flag_clear(DMA_USART, DMA_USART_CH, DMA_INT_FLAG_FTF);
		// g_recv_complete_flag = 1;
	}
}