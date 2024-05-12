/************************************************
 * @file Name: bsp_dma.c
 * @brief : DMA配置源文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-12
 * 
 * modification history :
 * @date:       @version:      @author:     
 * Changes: 
*************************************************/

#include "bsp_dma.h"
#include "usart.h"
/************************************************
 * @brief : 串口DMA的初始化函数
 * @date : 2024-05-12
*************************************************/
void dma_config(void)
{
	dma_single_data_parameter_struct dma_init_struct;//定义DMA参数结构体
	rcu_periph_clock_enable(BSP_DMA_RCU);//开启DMA时钟
	dma_deinit(BSP_DMA, BSP_DMA_CH);//复位DMA

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
	dma_single_data_mode_init(BSP_DMA, BSP_DMA_CH, &dma_init_struct);
	/*使能通道外设*/
	dma_channel_subperipheral_select(BSP_DMA, BSP_DMA_CH, DMA_SUBPERI4);
	/*使能DMA通道*/
	dma_channel_enable(BSP_DMA, BSP_DMA_CH);
	/*使能DMA中断*/
	dma_interrupt_enable(BSP_DMA, BSP_DMA_CH, DMA_CHXCTL_FTFIE);//通道传输完成中断
	nvic_irq_enable(BSP_DMA_CH_IRQ, 2, 1);
	/*使能串口接收DMA*/
	usart_dma_receive_config(USART0, USART_DENR_ENABLE);
}


/************************************************
 * @brief : 中断服务函数，在DMA传输完成时自动调用
 * @date : 2024-05-12
*************************************************/
void BSP_DMA_CH_IRQ_HANDLER(void)
{
	if (dma_interrupt_flag_get(BSP_DMA, BSP_DMA_CH, DMA_INT_FLAG_FTF) == SET)
	{
		dma_interrupt_flag_clear(BSP_DMA, BSP_DMA_CH, DMA_INT_FLAG_FTF);
		// g_recv_complete_flag = 1;
	}
}