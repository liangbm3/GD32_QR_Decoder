#include "bsp_key.h"
#include "bsp_led.h"
#include "stdio.h"

void key1_gpio_config(void)
{
	/* 开启时钟 */
	rcu_periph_clock_enable(BSP_KEY1_RCU);
	rcu_periph_clock_enable(RCU_SYSCFG);

	/* 配置GPIO的模式 */
	gpio_mode_set(BSP_KEY1_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BSP_KEY1_PIN);
}

void key2_gpio_config(void)
{
	/* 开启时钟 */
	rcu_periph_clock_enable(BSP_KEY2_RCU);
	rcu_periph_clock_enable(RCU_SYSCFG);

	/* 配置GPIO的模式 */
	gpio_mode_set(BSP_KEY2_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BSP_KEY2_PIN);
}

uint8_t key1_scan(void)
{
	/* 获取按键引脚的电平状态 */
	if (gpio_input_bit_get(BSP_KEY1_PORT, BSP_KEY1_PIN) == RESET)
	{
		delay_1ms(50);
		if (gpio_input_bit_get(BSP_KEY1_PORT, BSP_KEY1_PIN) == RESET)
		{
			while (gpio_input_bit_get(BSP_KEY1_PORT, BSP_KEY1_PIN) == RESET)
				; // 检测按键松开
			return 1;
		}
	}
	return 0;
}

uint8_t key2_scan(void)
{
	/* 获取按键引脚的电平状态 */
	if (gpio_input_bit_get(BSP_KEY2_PORT, BSP_KEY2_PIN) == RESET)
	{
		delay_1ms(50);
		if (gpio_input_bit_get(BSP_KEY2_PORT, BSP_KEY2_PIN) == RESET)
		{

			while (gpio_input_bit_get(BSP_KEY2_PORT, BSP_KEY2_PIN) == RESET)
				; // 检测按键松开
			return 1;
		}
	}
	return 0;
}