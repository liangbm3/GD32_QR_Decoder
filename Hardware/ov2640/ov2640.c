/************************************************
 * @file Name: ov2640.c
 * @brief : OV2640源文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-09
 *
 * modification history :
 * @date:       @version:      @author:
 * Changes:
 *************************************************/

#include "ov2640.h"
#include "ov2640_init_table.h"
#include "gd32f4xx_dma.h"

int capture_flag = 0;

/************************************************
 * @brief : OV2640摄像头初始化函数
 * @date : 2024-05-12
 *************************************************/
void DCI_OV2640_Init(void)
{
    /*开启GPIO时钟和DCI的时钟*/
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOG);
    rcu_periph_clock_enable(RCU_DCI);

    /*配置引脚复用，配置为DCI模式 */
    gpio_af_set(GPIOA, GPIO_AF_13, GPIO_PIN_4);
    gpio_af_set(GPIOA, GPIO_AF_13, GPIO_PIN_6);
    gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_6);
    gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_7);
    gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_8);
    gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_9);
    gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_11);
    gpio_af_set(GPIOB, GPIO_AF_13, GPIO_PIN_8);
    gpio_af_set(GPIOB, GPIO_AF_13, GPIO_PIN_9);
    gpio_af_set(GPIOD, GPIO_AF_13, GPIO_PIN_3);
    gpio_af_set(GPIOG, GPIO_AF_13, GPIO_PIN_9);

    /* configure DCI_PIXCLK(PA6), DCI_VSYNC(PG9), DCI_HSYNC(PA4), DCI_D0(PC6), DCI_D1(PC7)
                 DCI_D2(PC8), DCI_D3(PC9), DCI_D4(PC11), DCI_D5(PD3), DCI_D6(PB8), DCI_D7(PB9) */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_4);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_6);

    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_6);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_7);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_8);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_9);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_11);

    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_8);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_9);

    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_3);

    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_9);

    /**DCI参数结构体*/
    dci_parameter_struct dci_struct = {0};
    dci_deinit(); // DCI复位
    /* DCI configuration */
    dci_struct.capture_mode = DCI_CAPTURE_MODE_SNAPSHOT;      // 配置为连续捕获
    dci_struct.clock_polarity = DCI_CK_POLARITY_RISING;       // 时钟极性PCLK上升沿有效
    dci_struct.hsync_polarity = DCI_HSYNC_POLARITY_LOW;       // 水平参考信号低电平有效
    dci_struct.vsync_polarity = DCI_VSYNC_POLARITY_LOW;       // 垂直参考信号低电平有效
    dci_struct.frame_rate = DCI_FRAME_RATE_ALL;               // 全帧捕获
    dci_struct.interface_format = DCI_INTERFACE_FORMAT_8BITS; // 8位数据
    dci_init(&dci_struct);
}

/************************************************
 * @brief : OV2640SCCB协议初始化
 * @return uint8_t    0，成功；1，失败
 * @date : 2024-05-12
 *************************************************/
uint8_t SCCB_OV2640_init(void)
{
    uint16_t reg = 0;
    uint8_t i, dat = 0, ret = 0;
    rcu_periph_clock_enable(RCU_PWDN);
    rcu_periph_clock_enable(RCU_RST);

    // PWDN
    gpio_mode_set(PORT_PWDN, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PWDN);
    gpio_output_options_set(PORT_PWDN, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PWDN);

    // RST
    gpio_mode_set(PORT_RST, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_RST);
    gpio_output_options_set(PORT_RST, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_RST);

    OV2640_PWDN_OUT(0); // POWER ON
    delay_1ms(10);
    OV2640_RST_OUT(0); // 复位OV2640
    delay_1ms(10);
    OV2640_RST_OUT(1); // 结束复位

    OV2640_IIC_Init();
    ret = IIC_WR_Reg(0xFF, 0x01); // 操作sensor寄存器
    if (ret != 0)
        printf("error1 ret = %d\r\n", ret);

    ret = IIC_WR_Reg(0x12, 0x80); // 软复位OV2640
    if (ret != 0)
        printf("error2 ret = %d\r\n", ret);

    for (i = 0; i < sizeof(ov2640_svga_init_reg_tbl) / 2; i++)
    {
        IIC_WR_Reg(ov2640_svga_init_reg_tbl[i][0], ov2640_svga_init_reg_tbl[i][1]);
    }

    for (i = 0; i < (sizeof(ov2640_rgb565_reg_tbl) / 2); i++)
    {
        IIC_WR_Reg(ov2640_rgb565_reg_tbl[i][0], ov2640_rgb565_reg_tbl[i][1]);
    }
    return ret;
}

/************************************************
 * @brief : 设置图像输出大小，OV2640的输出大小，完全由该函数确定
 * @param  width  宽度，必须为4的倍数
 * @param  height  高度，必须为4的倍数
 * @return uint8_t
 * @date : 2024-05-12
 *************************************************/
uint8_t OV2640_Outsize_Set(uint16_t width, uint16_t height)
{
    uint16_t outh;
    uint16_t outw;
    uint8_t temp;
    if (width % 4)
        return 0xFF;
    if (height % 4)
        return 0xFF;
    outw = width / 4;
    outh = height / 4;
    IIC_WR_Reg(0xFF, 0x00);
    IIC_WR_Reg(0xE0, 0x04);        // 选择DVP
    IIC_WR_Reg(0x5A, outw & 0xFF); // 输出的宽      是真实的图像大小/4
    IIC_WR_Reg(0x5B, outh & 0xFF); // 输出的高      是真实的图像大小/4
    temp = (outw >> 8) & 0x03;
    temp |= (outh >> 6) & 0x04;
    IIC_WR_Reg(0x5C, temp); // bit[7:4]:ZMSPD    bit[2]: OUTH[8]    bit[1:0]: OUTW[9:8]
    IIC_WR_Reg(0xE0, 0x00); // 都复位
    return 0;
}

/*!
    \brief      read the ov2640 manufacturer identifier
    \param[in]  ov2640id: pointer to the ov2640 manufacturer struct
    \param[out] none
    \retval     0x00 or 0xFF
*/
uint8_t dci_ov2640_id_read(ov2640_id_struct *ov2640id)
{
    uint8_t temp;
    IIC_WR_Reg(0xFF, 0x01); // 选择table13寄存器组
    temp = IIC_RD_Reg(OV2640_MIDH);
    ov2640id->manufacturer_id1 = temp;
    temp = IIC_RD_Reg(OV2640_MIDL);
    ov2640id->manufacturer_id2 = temp;
    temp = IIC_RD_Reg(OV2640_VER);
    ov2640id->version = temp;
    temp = IIC_RD_Reg(OV2640_PID);
    ov2640id->pid = temp;
    return 0x00;
}

/************************************************
 * @brief : 设置图像的尺寸大小，也就是所选格式的输出分辨率
 * @param  width    图像的宽度
 * @param  height   图像的高度
 * @return uint8_t  0，设置成功；其他，设置失败
 * @date : 2024-05-20
 *************************************************/
uint8_t OV2640_ImageSize_Set(uint16_t width, uint16_t height)
{
    uint8_t temp;
    IIC_WR_Reg(0XFF, 0X00);
    IIC_WR_Reg(0XE0, 0X04);
    IIC_WR_Reg(0XC0, (width) >> 3 & 0XFF);  // 设置HSIZE的10：3位
    IIC_WR_Reg(0XC1, (height) >> 3 & 0XFF); // 设置VSIZE的10：3位
    temp = (width & 0X07) << 3;
    temp |= height & 0X07;
    temp |= (width >> 4) & 0X80;
    IIC_WR_Reg(0X8C, temp);
    IIC_WR_Reg(0XE0, 0X00);
    return 0;
}

void DCI_IRQHandler(void)
{
    if (dci_interrupt_flag_get(DCI_INT_FLAG_EF) == SET) // ²¶»ñÍê³ÉÒ»Ö¡Í¼Ïñ
    {
        capture_flag = 1;
        dci_interrupt_flag_clear(DCI_INT_EF); // Çå³ýÖ¡ÖÐ¶Ï
    }
}