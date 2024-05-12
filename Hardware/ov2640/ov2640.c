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

void DCI_OV2640_Init(void)
{
    dci_parameter_struct dci_struct={0};
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOG);
    rcu_periph_clock_enable(RCU_DCI);

    /* DCI GPIO AF configuration */
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
    
    dci_deinit();//Çå³ýÔ­ÅäÖÃ
    /* DCI configuration */
    dci_struct.capture_mode = DCI_CAPTURE_MODE_CONTINUOUS;    // Á¬ÐøÄ£Ê½
    // dci_struct.capture_mode=DCI_CAPTURE_MODE_SNAPSHOT;     // ¿ìÕÕ
    dci_struct.clock_polarity = DCI_CK_POLARITY_RISING;       // Ê±ÖÓ¼«ÐÔ PCLK ÉÏÉýÑØÓÐÐ§
    dci_struct.hsync_polarity = DCI_HSYNC_POLARITY_LOW;       // Ë®Æ½¼«ÐÔ HSYNC µÍµçÆ½ÓÐÐ§
    dci_struct.vsync_polarity = DCI_VSYNC_POLARITY_LOW;       // ´¹Ö±¼«ÐÔ VSYNC µÍµçÆ½ÓÐÐ§
    dci_struct.frame_rate = DCI_FRAME_RATE_ALL;               // È«Ö¡²¶»ñ
    dci_struct.interface_format = DCI_INTERFACE_FORMAT_8BITS; // 8Î»Êý¾Ý
    dci_init(&dci_struct);
    
}

uint8_t SCCB_OV2640_init(void)
{
   uint16_t reg = 0;
    uint8_t i,dat=0,ret=0;
    
    rcu_periph_clock_enable(RCU_GPIOA);

    //PWDN
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_7);
   
    //RST
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_5);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5);
    
    
    OV2640_PWDN_OUT(0);	//POWER ON
	delay_1ms(10);
	OV2640_RST_OUT(0);	//¸´Î»OV2640
	delay_1ms(10);
	OV2640_RST_OUT(1);	//½áÊø¸´Î» 
    
    OV2640_IIC_Init();
	ret = IIC_WR_Reg(0xFF, 0x01);	//²Ù×÷sensor¼Ä´æÆ÷
    if( ret != 0 )   printf("error1 ret = %d\r\n",ret);
    
 	ret = IIC_WR_Reg(0x12, 0x80);	//Èí¸´Î»OV2640
    if( ret != 0 )   printf("error2 ret = %d\r\n",ret);
    
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
    IIC_WR_Reg(0xE0, 0x04);        // Ñ¡ÔñDVP
    IIC_WR_Reg(0x5A, outw & 0xFF); // Êä³öµÄ¿í    ÊÇÕæÊµµÄÍ¼Ïñ´óÐ¡/4
    IIC_WR_Reg(0x5B, outh & 0xFF); // Êä³öµÄ¸ß		 ÊÇÕæÊµµÄÍ¼Ïñ´óÐ¡/4
    temp = (outw >> 8) & 0x03;
    temp |= (outh >> 6) & 0x04;
    IIC_WR_Reg(0x5C, temp); // bit[7:4]:ZMSPD    bit[2]: OUTH[8]    bit[1:0]: OUTW[9:8]
    IIC_WR_Reg(0xE0, 0x00); // ¶¼¸´Î»
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
    IIC_WR_Reg(0xFF, 0x01); // Ñ¡Ôñtable13¼Ä´æÆ÷×é
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
//ÉèÖÃÍ¼ÏñÊä³ö´óÐ¡
//OV2640Êä³öÍ¼ÏñµÄ´óÐ¡(·Ö±æÂÊ),ÍêÈ«ÓÉ¸Äº¯ÊýÈ·¶¨
//width,height:¿í¶È(¶ÔÓ¦:horizontal)ºÍ¸ß¶È(¶ÔÓ¦:vertical),widthºÍheight±ØÐëÊÇ4µÄ±¶Êý
//·µ»ØÖµ:0,ÉèÖÃ³É¹¦
//    ÆäËû,ÉèÖÃÊ§°Ü
uint8_t OV2640_OutSize_Set(uint16_t width,uint16_t height)
{
	uint16_t outh;
	uint16_t outw;       
	uint8_t temp; 
	if(width%4)return 1;
	if(height%4)return 2;
	outw=width/4;
	outh=height/4; 
	IIC_WR_Reg(0XFF,0X00);	
	IIC_WR_Reg(0XE0,0X04);			
	IIC_WR_Reg(0X5A,outw&0XFF);		//ÉèÖÃOUTWµÄµÍ°ËÎ»
	IIC_WR_Reg(0X5B,outh&0XFF);		//ÉèÖÃOUTHµÄµÍ°ËÎ»
	temp=(outw>>8)&0X03;
	temp|=(outh>>6)&0X04;
	IIC_WR_Reg(0X5C,temp);				//ÉèÖÃOUTH/OUTWµÄ¸ßÎ» 
	IIC_WR_Reg(0XE0,0X00);	
	return 0;
}

//¸Ãº¯ÊýÉèÖÃÍ¼Ïñ³ß´ç´óÐ¡,Ò²¾ÍÊÇËùÑ¡¸ñÊ½µÄÊä³ö·Ö±æÂÊ
//UXGA:1600*1200,SVGA:800*600,CIF:352*288
//width,height:Í¼Ïñ¿í¶ÈºÍÍ¼Ïñ¸ß¶È
//·µ»ØÖµ:0,ÉèÖÃ³É¹¦
//    ÆäËû,ÉèÖÃÊ§°Ü
uint8_t OV2640_ImageSize_Set(uint16_t width,uint16_t height)
{ 
	uint8_t temp; 
	IIC_WR_Reg(0XFF,0X00);			
	IIC_WR_Reg(0XE0,0X04);			
	IIC_WR_Reg(0XC0,(width)>>3&0XFF);		//ÉèÖÃHSIZEµÄ10:3Î»
	IIC_WR_Reg(0XC1,(height)>>3&0XFF);		//ÉèÖÃVSIZEµÄ10:3Î»
	temp=(width&0X07)<<3;
	temp|=height&0X07;
	temp|=(width>>4)&0X80; 
	IIC_WR_Reg(0X8C,temp);	
	IIC_WR_Reg(0XE0,0X00);				 
	return 0;
}