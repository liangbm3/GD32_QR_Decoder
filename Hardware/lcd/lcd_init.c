/************************************************
 * @file Name: lcd_init.c
 * @brief :lcd初始化源文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-15
 *
 * modification history :
 * @date:       @version:      @author:
 * Changes:
 *************************************************/
#include "lcd_init.h"

/************************************************
 * @brief : LCD的端口初始化函数
 * @date : 2024-05-15
 *************************************************/
void LCD_GPIO_Init(void)
{
#if !USE_HARDWARE
	/* 使能时钟 */
	rcu_periph_clock_enable(RCU_LCD_SCL);
	rcu_periph_clock_enable(RCU_LCD_SDA);
	rcu_periph_clock_enable(RCU_LCD_CS);
	rcu_periph_clock_enable(RCU_LCD_DC);
	rcu_periph_clock_enable(RCU_LCD_RES);
	rcu_periph_clock_enable(RCU_LCD_BLK);

	/* 配置SCL */
	gpio_mode_set(PORT_LCD_SCL, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_SCL);
	gpio_output_options_set(PORT_LCD_SCL, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_SCL);
	gpio_bit_write(PORT_LCD_SCL, GPIO_LCD_SCL, SET);

	/* 配置SDA */
	gpio_mode_set(PORT_LCD_SDA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_SDA);
	gpio_output_options_set(PORT_LCD_SDA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_SDA);
	gpio_bit_write(PORT_LCD_SDA, GPIO_LCD_SDA, SET);

	/* 配置DC */
	gpio_mode_set(PORT_LCD_DC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_DC);
	gpio_output_options_set(PORT_LCD_DC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_DC);
	gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET);

	/* 配置CS */
	gpio_mode_set(PORT_LCD_CS, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_CS);
	gpio_output_options_set(PORT_LCD_CS, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_CS);
	gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET);

	/* 配置RES */
	gpio_mode_set(PORT_LCD_RES, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_RES);
	gpio_output_options_set(PORT_LCD_RES, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_RES);
	gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET);

	/* 配置BLK */
	gpio_mode_set(PORT_LCD_BLK, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_BLK);
	gpio_output_options_set(PORT_LCD_BLK, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_BLK);
	gpio_bit_write(PORT_LCD_BLK, GPIO_LCD_BLK, SET);
#else
	/*开启时钟*/
	rcu_periph_clock_enable(RCU_LCD_SCL);
	rcu_periph_clock_enable(RCU_LCD_SDA);
	rcu_periph_clock_enable(RCU_LCD_CS);
	rcu_periph_clock_enable(RCU_LCD_DC);
	rcu_periph_clock_enable(RCU_LCD_RES);
	rcu_periph_clock_enable(RCU_LCD_BLK);
	rcu_periph_clock_enable(RCU_SPI_HARDWARE); // 开启SPI时钟

	/*配置SPI的SCK GPIO*/
	gpio_af_set(PORT_LCD_SCL, LINE_AF_SPI, GPIO_LCD_SCL);
	gpio_mode_set(PORT_LCD_SCL, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_SCL);
	gpio_output_options_set(PORT_LCD_SCL, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_SCL);
	gpio_bit_set(PORT_LCD_SCL, GPIO_LCD_SCL);

	/*配置SPI的MOSI GPIO*/
	gpio_af_set(PORT_LCD_SDA, LINE_AF_SPI, GPIO_LCD_SDA);
	gpio_mode_set(PORT_LCD_SDA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_SDA);
	gpio_output_options_set(PORT_LCD_SDA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_SDA);
	gpio_bit_set(PORT_LCD_SDA, GPIO_LCD_SDA);

	/* 配置DC */
	gpio_mode_set(PORT_LCD_DC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_DC);
	gpio_output_options_set(PORT_LCD_DC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_DC);
	gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET);

	/* 配置CS */
	gpio_mode_set(PORT_LCD_CS, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_LCD_CS);
	gpio_output_options_set(PORT_LCD_CS, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_CS);
	gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET);

	/* 配置RES */
	gpio_mode_set(PORT_LCD_RES, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_RES);
	gpio_output_options_set(PORT_LCD_RES, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_RES);
	gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET);

	/* 配置BLK */
	gpio_mode_set(PORT_LCD_BLK, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_BLK);
	gpio_output_options_set(PORT_LCD_BLK, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_BLK);
	gpio_bit_write(PORT_LCD_BLK, GPIO_LCD_BLK, SET);

	/*配置SPI参数*/
	spi_parameter_struct spi_init_struct;
	spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX; // 配置为传输模式全通工
	spi_init_struct.device_mode = SPI_MASTER;			   // 配置为主机
	spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;	   // 8位数据
	spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
	spi_init_struct.nss = SPI_NSS_SOFT;	  // 软件CS
	spi_init_struct.prescale = SPI_PSC_2; // 2分频
	spi_init_struct.endian = SPI_ENDIAN_MSB;
	spi_init(PORT_SPI, &spi_init_struct);

	/*使能SPI*/
	spi_enable(PORT_SPI);
#endif
}

/************************************************
 * @brief : 串行写入数据
 * @param  dat 要写入的数据
 * @date : 2024-05-15
 *************************************************/
void LCD_Writ_Bus(u8 dat)
{
#if !USE_HARDWARE
	u8 i;
	LCD_CS_Clr();
	for (i = 0; i < 8; i++)
	{
		LCD_SCLK_Clr();
		if (dat & 0x80)
		{
			LCD_MOSI_Set();
		}
		else
		{
			LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat <<= 1;
	}
	LCD_CS_Set();
#else
	LCD_CS_Clr();
	while (RESET == spi_i2s_flag_get(PORT_SPI, SPI_FLAG_TBE))
		;
	spi_i2s_data_transmit(PORT_SPI, dat);
	while (RESET == spi_i2s_flag_get(PORT_SPI, SPI_FLAG_RBNE))
		;
	spi_i2s_data_receive(PORT_SPI);
	LCD_CS_Set();
#endif
}

/******************************************************************************
	  函数说明：LCD写入数据
	  入口数据：dat 写入的数据
	  返回值：  无
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_Writ_Bus(dat);
}

/******************************************************************************
	  函数说明：LCD写入数据
	  入口数据：dat 写入的数据
	  返回值：  无
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
	LCD_Writ_Bus(dat >> 8);
	LCD_Writ_Bus(dat);
}

/******************************************************************************
	  函数说明：LCD写入命令
	  入口数据：dat 写入的命令
	  返回值：  无
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	LCD_DC_Clr(); // 写命令
	LCD_Writ_Bus(dat);
	LCD_DC_Set(); // 写数据
}

/******************************************************************************
	  函数说明：设置起始和结束地址
	  入口数据：x1,x2 设置列的起始和结束地址
				y1,y2 设置行的起始和结束地址
	  返回值：  无
******************************************************************************/
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2)
{
	if (USE_HORIZONTAL == 0)
	{
		LCD_WR_REG(0x2a); // 列地址设置
		LCD_WR_DATA(x1 + 2);
		LCD_WR_DATA(x2 + 2);
		LCD_WR_REG(0x2b); // 行地址设置
		LCD_WR_DATA(y1 + 1);
		LCD_WR_DATA(y2 + 1);
		LCD_WR_REG(0x2c); // 储存器写
	}
	else if (USE_HORIZONTAL == 1)
	{
		LCD_WR_REG(0x2a); // 列地址设置
		LCD_WR_DATA(x1 + 2);
		LCD_WR_DATA(x2 + 2);
		LCD_WR_REG(0x2b); // 行地址设置
		LCD_WR_DATA(y1 + 1);
		LCD_WR_DATA(y2 + 1);
		LCD_WR_REG(0x2c); // 储存器写
	}
	else if (USE_HORIZONTAL == 2)
	{
		LCD_WR_REG(0x2a); // 列地址设置
		LCD_WR_DATA(x1 + 1);
		LCD_WR_DATA(x2 + 1);
		LCD_WR_REG(0x2b); // 行地址设置
		LCD_WR_DATA(y1 + 2);
		LCD_WR_DATA(y2 + 2);
		LCD_WR_REG(0x2c); // 储存器写
	}
	else
	{
		LCD_WR_REG(0x2a); // 列地址设置
		LCD_WR_DATA(x1 + 1);
		LCD_WR_DATA(x2 + 1);
		LCD_WR_REG(0x2b); // 行地址设置
		LCD_WR_DATA(y1 + 2);
		LCD_WR_DATA(y2 + 2);
		LCD_WR_REG(0x2c); // 储存器写
	}
}

void LCD_Init(void)
{
	LCD_GPIO_Init(); // 初始化GPIO

	LCD_RES_Clr(); // 复位
	delay_ms(100);
	LCD_RES_Set();
	delay_ms(100);

	LCD_BLK_Set(); // 打开背光
	delay_ms(100);

	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); // Sleep out
	delay_ms(120);	  // Delay 120ms
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
	LCD_WR_REG(0xB1);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB3);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	//------------------------------------End ST7735S Frame Rate---------------------------------//
	LCD_WR_REG(0xB4); // Dot inversion
	LCD_WR_DATA8(0x03);
	//------------------------------------ST7735S Power Sequence---------------------------------//
	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x04);
	LCD_WR_REG(0xC1);
	LCD_WR_DATA8(0XC0);
	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0x2A);
	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	LCD_WR_REG(0xC5); // VCOM
	LCD_WR_DATA8(0x1A);
	LCD_WR_REG(0x36); // MX, MY, RGB mode
	if (USE_HORIZONTAL == 0)
		LCD_WR_DATA8(0x00);
	else if (USE_HORIZONTAL == 1)
		LCD_WR_DATA8(0xC0);
	else if (USE_HORIZONTAL == 2)
		LCD_WR_DATA8(0x70);
	else
		LCD_WR_DATA8(0xA0);
	//------------------------------------ST7735S Gamma Sequence---------------------------------//
	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x22);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x30);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2A);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x3A);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x13);
	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x16);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x23);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x3B);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x13);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------//
	LCD_WR_REG(0x3A); // 65k mode
	LCD_WR_DATA8(0x05);
	LCD_WR_REG(0x29); // Display on
}
