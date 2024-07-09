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
#include "stdlib.h"
#include "string.h"
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "lcd.h"
#include "lcd_init.h"
#include "pic.h"
#include "sccb.h"
#include "ov2640.h"
#include "zbar.h"
#include "bsp_key.h"

#define PHOTO_WIDTH 100                         // 图片宽
#define PHOTO_HEIGHT 100                        // 图片高
#define PHOTO_SIZE (PHOTO_WIDTH * PHOTO_HEIGHT) // 图片大小
#define THRESHOLD 128                           // 二值化的阈值

void sound_config(void);
void sound_on1(void);
void sound_on2(void);

int Zbar_Test(void *raw, int width, int height);
void get_one_frame(void);
void show_binary_photo(void);
uint8_t get_brightness(uint16_t pixel);
uint8_t result_length = 4; // 结果长度
uint8_t color = 1;         // 0:黑白 1:彩色
uint8_t menu_state = 0;
uint8_t first_time=0;
int main()
{
    float t = 0;
    sound_config();
    key1_gpio_config();
    key2_gpio_config();
    usart_gpio_config(115200);
    esp_usart_gpio_config(115200);
    dma_config();
    systick_config();
    LCD_Init();
    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
    SCCB_OV2640_init();
    DCI_OV2640_Init();
    OV2640_ImageSize_Set(1000, 1000);
    OV2640_Outsize_Set(PHOTO_WIDTH, PHOTO_HEIGHT);
    nvic_irq_enable(DCI_IRQn, 0U, 0U);
    dci_interrupt_enable(DCI_INT_EF);
    dci_enable();
    uint8_t set_mode = 1;
    LCD_ShowPicture(0, 0, 128, 160, welcome);
    delay_1ms(1000);
    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
    while (1)
    {

        while (set_mode)
        {
            LCD_ShowString(10, 10, "SETTING", RED, WHITE, 32, 0);
            if(menu_state==0)
            {
                LCD_DrawLine(20,58,115,58,GREEN);
                LCD_DrawLine(20,58,20,78,GREEN); 
                LCD_DrawLine(115,58,115,78,GREEN);   
            }
            else
            {
                
                LCD_DrawLine(20,58,115,58,WHITE);   
                LCD_DrawLine(20,58,115,58,WHITE); 
                LCD_DrawLine(20,58,20,78,WHITE); 
                LCD_DrawLine(115,58,115,78,WHITE);  
            }
            LCD_DrawLine(20,78,115,78,GREEN);
            if(menu_state==1)
            {
                LCD_DrawLine(20,98,115,98,GREEN);
                LCD_DrawLine(20,78,20,98,GREEN);
                LCD_DrawLine(115,78,115,98,GREEN);    
            }
            LCD_ShowString(24, 60, "length", BLACK, WHITE, 16, 0);
            LCD_ShowIntNum(72, 60, result_length, 3, BLACK, WHITE, 16);
            LCD_ShowString(24, 80, "mode", BLACK, WHITE, 16, 0);
            if (color)
            {
                LCD_ShowString(72, 80, "color", BLACK, WHITE, 16, 0);
            }
            else
            {
                LCD_ShowString(72, 80, "black", BLACK, WHITE, 16, 0);
            }
            LCD_ShowString(10, 120, "ip:192.168.4.1/html", BLACK, WHITE, 12, 0);
            if (key1_scan() == 1)
            {
                if (menu_state == 0)
                {
                    if (result_length < 7)
                    {
                        result_length += 1;
                    }
                    else
                    {
                        result_length = 2;
                    }
                }
                else if (menu_state == 1)
                {
                    if (color == 1)
                    {
                        color = 0;
                    }
                    else
                    {
                        color = 1;
                    }
                }
            }
            if (key2_scan() == 1)
            {
                if (menu_state == 0)
                {
                    menu_state = 1;
                }
                else if (menu_state == 1)
                {
                    set_mode = 0;
                    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
                }
            }
        }
        LCD_ShowString(24, 105, "result:", BLACK, WHITE, 16, 0);
        if (color)
        {
            get_one_frame();
        }
        else
        {
            show_binary_photo();
        }
        if(key1_scan()==1||key2_scan()==1)
        {
            LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
            menu_state=0;
            set_mode=1;
        }

    }
}

void get_one_frame(void)
{
    uint32_t *photo_buff;
    photo_buff = (uint32_t *)malloc(PHOTO_WIDTH * PHOTO_HEIGHT * sizeof(uint32_t));
    DCItoLCD_DMA_Init(photo_buff);
    dci_capture_enable();
    delay_1ms(150);

    dma_deinit(DMA_DCI, DMA_DCI_CH);
    dci_capture_disable();
    u16 *p = (u16 *)photo_buff;
    u8 *binary_photo = (u8 *)malloc(PHOTO_WIDTH * PHOTO_HEIGHT);
    u32 index = 0;
    for (int i = 0; i < PHOTO_HEIGHT; i++)
    {
        for (int j = 0; j < PHOTO_WIDTH; j++)
        {
            LCD_DrawPoint(j + 1, i, p[index]);
            uint8_t brightness = get_brightness(p[index]);
            if (brightness > THRESHOLD)
            {
                p[index] = 0xFFFF;          // 白色                      //转换成的16位是用来屏幕输出的
                binary_photo[index] = 0xFF; // 白色                //转换成的8位是用来zbar检测的
            }
            else
            {
                p[index] = 0x0000;          // 黑色
                binary_photo[index] = 0x00; // 黑色
            }
            
            index += 1;
        }
    }
    free(photo_buff); // 在zbar检测之前释放掉photo_buff动态内存，不然内存不够用来检测
    Zbar_Test(binary_photo, PHOTO_WIDTH, PHOTO_HEIGHT);
}

/************************************************
 * @brief : 获取一帧图像，将该图像二值化后，显示到屏幕，并用zbar检测，将结果输出到串口
 * @date : 2024-06-12
 *************************************************/
void show_binary_photo(void)
{
    uint32_t *photo_buff;
    photo_buff = (uint32_t *)malloc(PHOTO_WIDTH * PHOTO_HEIGHT * sizeof(uint32_t)); // 由于单片机内存不够，这里先申请一个内存用来存十六位的RBG565图像数据
    DCItoLCD_DMA_Init(photo_buff);                                                  // 启动传输
    dci_capture_enable();
    delay_1ms(150);
    dma_deinit(DMA_DCI, DMA_DCI_CH);
    dci_capture_disable();
    u16 *p = (u16 *)photo_buff;
    u8 *binary_photo = (u8 *)malloc(PHOTO_WIDTH * PHOTO_HEIGHT);
    u32 index = 0;
    for (int i = 0; i < PHOTO_HEIGHT; i++)
    {
        for (int j = 0; j < PHOTO_WIDTH; j++)
        {
            uint8_t brightness = get_brightness(p[index]);
            if (brightness > THRESHOLD)
            {
                p[index] = 0xFFFF;          // 白色                      //转换成的16位是用来屏幕输出的
                binary_photo[index] = 0xFF; // 白色                //转换成的8位是用来zbar检测的
            }
            else
            {
                p[index] = 0x0000;          // 黑色
                binary_photo[index] = 0x00; // 黑色
            }
            LCD_DrawPoint(j + 1, i, p[index]);
            index += 1;
        }
    }
    free(photo_buff); // 在zbar检测之前释放掉photo_buff动态内存，不然内存不够用来检测
    Zbar_Test(binary_photo, PHOTO_WIDTH, PHOTO_HEIGHT);
}

/************************************************
 * @brief : 计算 RGB565 像素的亮度
 * @param  pixel   像素的值
 * @return uint8_t   像素的亮度值
 * @date : 2024-06-12
 *************************************************/
uint8_t get_brightness(uint16_t pixel)
{
    uint8_t r = (pixel >> 11) & 0x1F; // 提取红色分量
    uint8_t g = (pixel >> 5) & 0x3F;  // 提取绿色分量
    uint8_t b = pixel & 0x1F;         // 提取蓝色分量

    // 将分量转换到 [0, 255] 范围
    r = (r * 255) / 31;
    g = (g * 255) / 63;
    b = (b * 255) / 31;

    // 使用加权平均法计算亮度
    uint8_t brightness = (r * 0.299) + (g * 0.587) + (b * 0.114);
    return brightness;
}

/************************************************
 * @brief : 蜂鸣器初始化函数
 * @date : 2024-06-23
 *************************************************/
void sound_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOF);
    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_6);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
}

/************************************************
 * @brief : 蜂鸣器响一声
 * @date : 2024-06-23
 *************************************************/
void sound_on1(void)
{
    gpio_bit_set(GPIOF, GPIO_PIN_6);
    delay_1ms(2000);
    gpio_bit_reset(GPIOF, GPIO_PIN_6);
}

/************************************************
 * @brief : 蜂鸣器响两声
 * @date : 2024-06-23
 *************************************************/
void sound_on2(void)
{
    gpio_bit_set(GPIOF, GPIO_PIN_6);
    delay_1ms(20);
    gpio_bit_reset(GPIOF, GPIO_PIN_6);
    gpio_bit_set(GPIOF, GPIO_PIN_6);
    delay_1ms(20);
    gpio_bit_reset(GPIOF, GPIO_PIN_6);
}

/************************************************
 * @brief : Zbar测试函数
 * @param  raw   二维码图像指针
 * @param  width   图像宽度
 * @param  height  图像高度
 * @return int
 *************************************************/
int Zbar_Test(void *raw, int width, int height)
{

    zbar_image_scanner_t *scanner = NULL;
    /* create a reader */
    scanner = zbar_image_scanner_create();

    /* configure the reader */
    zbar_image_scanner_set_config(scanner, 64, ZBAR_CFG_ENABLE, 1);

    /* wrap image data */
    zbar_image_t *image = zbar_image_create();
    zbar_image_set_format(image, *(int *)"Y800");
    zbar_image_set_size(image, width, height);
    zbar_image_set_data(image, raw, width * height, zbar_image_free_data);

    /* scan the image for barcodes */
    int n = zbar_scan_image(scanner, image);
    if(n==0&first_time==0)
    {
        LCD_Fill(0,124,128,160,WHITE);
        LCD_ShowString(24, 125, "no result", BLACK, WHITE, 16, 0);
        first_time=1;
    }
    /* extract results */
    const zbar_symbol_t *symbol = zbar_image_first_symbol(image);
    for (; symbol; symbol = zbar_symbol_next(symbol))
    {
        /* do something useful with results */
        zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
        const char *data = zbar_symbol_get_data(symbol);
        printf("\r\ndecoded %s symbol \"%s\"\r\n",
               zbar_get_symbol_name(typ), data);
        LCD_Fill(0,124,128,160,WHITE);
        LCD_ShowString(24, 125,data, BLACK, WHITE, 16, 0);
        esp_send_string(data);
        if (strlen(data) > result_length)
        {
            sound_on1();
        }
        else
        {
            sound_on2();
        }
    }

    /* clean up */
    zbar_image_destroy(image);
    zbar_image_scanner_destroy(scanner);

    return (0);
}