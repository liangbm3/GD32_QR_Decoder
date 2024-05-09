/************************************************
 * @file Name: main.c
 * @brief : 
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-09
 * 
 * modification history :
 * @date:       @version:      @author:     
 * Changes: 
*************************************************/
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "bsp_led.h"

int main(void)
{
    systick_config();
	led_gpio_config();
	gpio_bit_write(PORT_LED2,PIN_LED2,SET);
    while(1) {
			delay_1ms(200);
			gpio_bit_write(PORT_LED2,PIN_LED2,RESET);
			delay_1ms(200);
			gpio_bit_write(PORT_LED2,PIN_LED2,SET);
    }
    
}
/**
 * @brief : 
 * @param  a                
 * @return int              
 * @author : lbm (3095088766@qq.com)
 * @date : 2024-05-09
 */
int tst(int a)
{

}
