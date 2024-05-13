/********************************************************************************
 * 测试硬件：立创·梁山派开发板GD32F470ZGT6    使用主频200Mhz    晶振25Mhz
 * 版 本 号: V1.0
 * 修改作者: LC
 * 修改日期: 2023年06月12日
 * 功能介绍:
 ******************************************************************************
 * 梁山派软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：club.szlcsc.com
 * 其余模块移植手册：【立创·梁山派开发板】模块移植手册
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 *********************************************************************************/

#include "lcdinit.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_spi.h"

/******************************************************************
 * 函 数 名 称：lcd_gpio_config
 * 函 数 说 明：对LCD引脚初始化
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：注意是使用软件SPI还是硬件SPI
 ******************************************************************/
void lcd_gpio_config(void)
{

#if USE_SOFTWARE

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

	spi_parameter_struct spi_init_struct;

	rcu_periph_clock_enable(RCU_LCD_SCL);
	rcu_periph_clock_enable(RCU_LCD_SDA);
	rcu_periph_clock_enable(RCU_LCD_CS);
	rcu_periph_clock_enable(RCU_LCD_DC);
	rcu_periph_clock_enable(RCU_LCD_RES);
	rcu_periph_clock_enable(RCU_LCD_BLK);

	rcu_periph_clock_enable(RCU_SPI_HARDWARE); // 使能SPI

	/* 配置 SPI的SCK GPIO */
	gpio_af_set(PORT_LCD_SCL, LINE_AF_SPI, GPIO_LCD_SCL);
	gpio_mode_set(PORT_LCD_SCL, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_SCL);
	gpio_output_options_set(PORT_LCD_SCL, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_SCL);
	gpio_bit_set(PORT_LCD_SCL, GPIO_LCD_SCL);
	/* 配置 SPI的MOSI  GPIO */
	gpio_af_set(PORT_LCD_SDA, LINE_AF_SPI, GPIO_LCD_SDA);
	gpio_mode_set(PORT_LCD_SDA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_SDA);
	gpio_output_options_set(PORT_LCD_SDA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_SDA);
	gpio_bit_set(PORT_LCD_SDA, GPIO_LCD_SDA);

	/* 配置DC */
	gpio_mode_set(PORT_LCD_DC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_DC);
	gpio_output_options_set(PORT_LCD_DC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_DC);
	gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET);
	/* 配置RES */
	gpio_mode_set(PORT_LCD_RES, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_RES);
	gpio_output_options_set(PORT_LCD_RES, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_RES);
	gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET);
	/* 配置BLK */
	gpio_mode_set(PORT_LCD_BLK, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_BLK);
	gpio_output_options_set(PORT_LCD_BLK, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_BLK);
	gpio_bit_write(PORT_LCD_BLK, GPIO_LCD_BLK, SET);
	/* 配置CS */
	gpio_mode_set(PORT_LCD_CS, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_LCD_CS);
	gpio_output_options_set(PORT_LCD_CS, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_CS);
	gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET);

	/* 配置 SPI 参数 */
	spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX; // 传输模式全双工
	spi_init_struct.device_mode = SPI_MASTER;			   // 配置为主机
	spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;	   // 8位数据
	spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
	spi_init_struct.nss = SPI_NSS_SOFT;	  // 软件cs
	spi_init_struct.prescale = SPI_PSC_2; // 2分频
	spi_init_struct.endian = SPI_ENDIAN_MSB;
	spi_init(PORT_SPI, &spi_init_struct);

	/* 使能 SPI */
	spi_enable(PORT_SPI);

#endif
}

/******************************************************************
 * 函 数 名 称：LCD_Writ_Bus
 * 函 数 说 明：LCD串行数据写入函数
 * 函 数 形 参：dat  要写入的串行数据
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：注意是使用软件SPI还是硬件SPI
 ******************************************************************/
void LCD_Writ_Bus(uint8_t dat)
{
#if USE_SOFTWARE
	uint8_t i;
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

/******************************************************************
 * 函 数 名 称：LCD_WR_DATA8
 * 函 数 说 明：LCD写入8位数据
 * 函 数 形 参：dat 写入的数据
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
 ******************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}

/******************************************************************
 * 函 数 名 称：LCD_WR_DATA
 * 函 数 说 明：LCD写入16位数据
 * 函 数 形 参：dat 写入的数据
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
 ******************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_Writ_Bus(dat >> 8);
	LCD_Writ_Bus(dat);
}

/******************************************************************
 * 函 数 名 称：LCD_WR_REG
 * 函 数 说 明：LCD写入命令
 * 函 数 形 参：dat 写入的命令
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
 ******************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr(); // 写命令
	LCD_Writ_Bus(dat);
	LCD_DC_Set(); // 写数据
}

/******************************************************************
 * 函 数 名 称：LCD_Address_Set
 * 函 数 说 明：设置起始和结束地址
 * 函 数 形 参：x1,x2 设置列的起始和结束地址
				y1,y2 设置行的起始和结束地址
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	if (USE_HORIZONTAL == 0)
	{
		LCD_WR_REG(0x2a); // 列地址设置
		LCD_WR_DATA(x1 + 35);
		LCD_WR_DATA(x2 + 35);
		LCD_WR_REG(0x2b); // 行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c); // 储存器写
	}
	else if (USE_HORIZONTAL == 1)
	{
		LCD_WR_REG(0x2a); // 列地址设置
		LCD_WR_DATA(x1 + 35);
		LCD_WR_DATA(x2 + 35);
		LCD_WR_REG(0x2b); // 行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c); // 储存器写
	}
	else if (USE_HORIZONTAL == 2)
	{
		LCD_WR_REG(0x2a); // 列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b); // 行地址设置
		LCD_WR_DATA(y1 + 35);
		LCD_WR_DATA(y2 + 35);
		LCD_WR_REG(0x2c); // 储存器写
	}
	else
	{
		LCD_WR_REG(0x2a); // 列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b); // 行地址设置
		LCD_WR_DATA(y1 + 35);
		LCD_WR_DATA(y2 + 35);
		LCD_WR_REG(0x2c); // 储存器写
	}
}

/******************************************************************
 * 函 数 名 称：LCD_Init
 * 函 数 说 明：LCD初始化
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
 ******************************************************************/
void LCD_Init(void)
{
	lcd_gpio_config(); // 初始化GPIO

	LCD_RES_Clr(); // 复位
	delay_1ms(30);
	LCD_RES_Set();
	delay_1ms(100);
	LCD_BLK_Set(); // 打开背光
	delay_1ms(100);
	//
	LCD_WR_REG(0x11);
	//	delay_ms(120);
	LCD_WR_REG(0x36);
	if (USE_HORIZONTAL == 0)
		LCD_WR_DATA8(0x00);
	else if (USE_HORIZONTAL == 1)
		LCD_WR_DATA8(0xC0);
	else if (USE_HORIZONTAL == 2)
		LCD_WR_DATA8(0x70);
	else
		LCD_WR_DATA8(0xA0);

	LCD_WR_REG(0x3A);
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33);

	LCD_WR_REG(0xB7);
	LCD_WR_DATA8(0x35);

	LCD_WR_REG(0xBB);
	LCD_WR_DATA8(0x1A);

	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x2C);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x0B);

	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x20);

	LCD_WR_REG(0xC6);
	LCD_WR_DATA8(0x0F);

	LCD_WR_REG(0xD0);
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1);

	LCD_WR_REG(0x21);
	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xF0);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x29);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x3E);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0x12);
	LCD_WR_DATA8(0x12);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x30);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0xF0);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x3E);
	LCD_WR_DATA8(0x36);
	LCD_WR_DATA8(0x14);
	LCD_WR_DATA8(0x14);
	LCD_WR_DATA8(0x29);
	LCD_WR_DATA8(0x32);

	LCD_WR_REG(0x11);
	delay_1ms(120);
	LCD_WR_REG(0x29);
}
