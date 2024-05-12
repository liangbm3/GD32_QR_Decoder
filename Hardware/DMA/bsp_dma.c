#include "bsp_dma.h"
#include "usart.h"

void dma_config(void)
{
	dma_single_data_parameter_struct dma_init_struct;
	rcu_periph_clock_enable(BSP_DMA_RCU);

	dma_deinit(BSP_DMA, BSP_DMA_CH);

	dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.memory0_addr = (uint32_t)g_recv_buff;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
	dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
	dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
	dma_init_struct.number = USART_RECEIVE_LENGTH;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;

	dma_single_data_mode_init(BSP_DMA, BSP_DMA_CH, &dma_init_struct);

	dma_channel_subperipheral_select(BSP_DMA, BSP_DMA_CH, DMA_SUBPERI4);

	dma_channel_enable(BSP_DMA, BSP_DMA_CH);

	dma_interrupt_enable(BSP_DMA, BSP_DMA_CH, DMA_CHXCTL_FTFIE);
	nvic_irq_enable(BSP_DMA_CH_IRQ, 2, 1);

	usart_dma_receive_config(USART0, USART_DENR_ENABLE);
}

void BSP_DMA_CH_IRQ_HANDLER(void)
{
	if (dma_interrupt_flag_get(BSP_DMA, BSP_DMA_CH, DMA_INT_FLAG_FTF) == SET)
	{
		dma_interrupt_flag_clear(BSP_DMA, BSP_DMA_CH, DMA_INT_FLAG_FTF);
		// g_recv_complete_flag = 1;
	}
}