#include "bsp_usart.h"
#include "stdio.h"
#include "bsp_dma.h"



uint8_t g_recv_buff[USART_RECEIVE_LENGTH];
uint16_t g_recv_length = 0;
uint8_t g_recv_complete_flag = 0; 

void usart_gpio_config(uint32_t band_rate)
{
  /* ����ʱ�� */
	rcu_periph_clock_enable(BSP_USART_TX_RCU); 
	rcu_periph_clock_enable(BSP_USART_RX_RCU); 
	rcu_periph_clock_enable(BSP_USART_RCU); 
	
	/* ����GPIO���ù��� */
  gpio_af_set(BSP_USART_TX_PORT,BSP_USART_AF,BSP_USART_TX_PIN);	
	gpio_af_set(BSP_USART_RX_PORT,BSP_USART_AF,BSP_USART_RX_PIN);	
	
	/* ����GPIO��ģʽ */
	/* ����TXΪ����ģʽ ����ģʽ */
	gpio_mode_set(BSP_USART_TX_PORT,GPIO_MODE_AF,GPIO_PUPD_PULLUP,BSP_USART_TX_PIN);
	/* ����RXΪ����ģʽ ����ģʽ */
	gpio_mode_set(BSP_USART_RX_PORT, GPIO_MODE_AF,GPIO_PUPD_PULLUP,BSP_USART_RX_PIN);
	
	/* ����TXΪ������� 50MHZ */
	gpio_output_options_set(BSP_USART_TX_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BSP_USART_TX_PIN);
	/* ����RXΪ������� 50MHZ */
	gpio_output_options_set(BSP_USART_RX_PORT,GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BSP_USART_RX_PIN);

	/* ���ô��ڵĲ��� */
	usart_deinit(BSP_USART);
	usart_baudrate_set(BSP_USART,band_rate);
	usart_parity_config(BSP_USART,USART_PM_NONE);
	usart_word_length_set(BSP_USART,USART_WL_8BIT);
	usart_stop_bit_set(BSP_USART,USART_STB_1BIT);

  /* ʹ�ܴ��� */
	usart_enable(BSP_USART);
	usart_transmit_config(BSP_USART,USART_TRANSMIT_ENABLE);
	usart_receive_config(BSP_USART,USART_RECEIVE_ENABLE);
	
	nvic_irq_enable(BSP_USART_IRQ, 2, 2); 
	
	#if !USB_USART_DMA
 	usart_interrupt_enable(BSP_USART,USART_INT_RBNE);
	#endif
	
	usart_interrupt_enable(BSP_USART,USART_INT_IDLE);
}

/* ���ͺ��� */
void usart_send_data(uint8_t ucch)
{
	usart_data_transmit(BSP_USART,(uint8_t)ucch);
	while(RESET == usart_flag_get(BSP_USART,USART_FLAG_TBE));
}


/* ���ڷ����ַ��� */
void usart_send_string(uint8_t *ucstr)
{
	while(ucstr && *ucstr)
	{
	  usart_send_data(*ucstr++);
	}
}

int fputc(int ch, FILE *f)
{
     usart_send_data(ch);
     // �ȴ��������ݻ�������־��λ
     return ch;
}


void USART0_IRQHandler(void)
{
	#if !USB_USART_DMA
	if(usart_interrupt_flag_get(BSP_USART,USART_INT_FLAG_RBNE) == SET)
	{
		g_recv_buff[g_recv_length++] = usart_data_receive(BSP_USART);
	}
	#endif
	
	if(usart_interrupt_flag_get(BSP_USART,USART_INT_FLAG_IDLE) == SET)
	{
		usart_data_receive(BSP_USART);

		#if USB_USART_DMA
			g_recv_length = USART_RECEIVE_LENGTH - dma_transfer_number_get(BSP_DMA,BSP_DMA_CH);
			dma_channel_disable(BSP_DMA,BSP_DMA_CH);
			dma_config();
		#endif
		
		g_recv_buff[g_recv_length] = '\0';
		g_recv_complete_flag = 1;
		
	}
	
}