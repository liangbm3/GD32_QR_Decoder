/************************************************
 * @file Name: bsp_led.c
 * @brief : 
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-09
 * 
 * modification history :
 * @date:       @version:      @author:     
 * Changes: 
*************************************************/
#include "bsp_led.h"

/************************************************
 * @brief : LDE初始化函数
 * @date : 2024-05-09
*************************************************/
void led_gpio_config(void)
{
	/* 使能时钟 */
	rcu_periph_clock_enable(RCU_LED2);
	/* 配置为输出模式 浮空模式 */
	gpio_mode_set(PORT_LED2, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED2);
	/* 配置为推挽输出 50MHZ */
	gpio_output_options_set(PORT_LED2, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PIN_LED2);
}
