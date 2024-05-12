/************************************************
 * @file Name: led.c
 * @brief : 板载LED灯源文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-09
 * 
 * modification history :
 * @date:       @version:      @author:     
 * Changes: 
*************************************************/
#include "led.h"

/************************************************
 * @brief : LED1初始化函数，设置为浮空推挽输出
 * @date : 2024-05-09
*************************************************/
void led1_gpio_init(void)
{
	/* 使能时钟 */
	rcu_periph_clock_enable(RCU_LED1);
	/* 配置为输出模式 浮空模式 */
	gpio_mode_set(PORT_LED1, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED1);
	/* 配置为推挽输出 50MHZ */
	gpio_output_options_set(PORT_LED1, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PIN_LED1);
}

/************************************************
 * @brief : LED2初始化函数，设置为浮空推挽输出
 * @date : 2024-05-09
*************************************************/
void led2_gpio_init(void)
{
	/* 使能时钟 */
	rcu_periph_clock_enable(RCU_LED2);
	/* 配置为输出模式 浮空模式 */
	gpio_mode_set(PORT_LED2, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED2);
	/* 配置为推挽输出 50MHZ */
	gpio_output_options_set(PORT_LED2, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PIN_LED2);
}

/************************************************
 * @brief : LED3初始化函数，设置为浮空推挽输出
 * @date : 2024-05-09
*************************************************/
void led3_gpio_init(void)
{
	/* 使能时钟 */
	rcu_periph_clock_enable(RCU_LED3);
	/* 配置为输出模式 浮空模式 */
	gpio_mode_set(PORT_LED3, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED3);
	/* 配置为推挽输出 50MHZ */
	gpio_output_options_set(PORT_LED3, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PIN_LED3);
}


/************************************************
 * @brief : LED4初始化函数，设置为浮空推挽输出
 * @date : 2024-05-09
*************************************************/
void led4_gpio_init(void)
{
	/* 使能时钟 */
	rcu_periph_clock_enable(RCU_LED4);
	/* 配置为输出模式 浮空模式 */
	gpio_mode_set(PORT_LED4, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED4);
	/* 配置为推挽输出 50MHZ */
	gpio_output_options_set(PORT_LED4, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PIN_LED4);
}


/************************************************
 * @brief : 初始化开发板上的四个LED灯，设置为浮空推挽输出
 * @date : 2024-05-09
*************************************************/
void led_gpio_init(void)
{
	led1_gpio_init();
	led2_gpio_init();
	led3_gpio_init();
	led4_gpio_init();
}
