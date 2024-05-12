/************************************************
 * @file Name: sccb.c
 * @brief : SCCB控制接口源文件
 * @author : lbm (3095088766@qq.com)
 * @version : 1.0
 * @date : 2024-05-09
 *
 * modification history :
 * @date:       @version:      @author:
 * Changes:
 *************************************************/
#include "sccb.h"

/************************************************
 * @brief : OV2640的SCCB接口初始化
 * @date : 2024-05-09
 *************************************************/
void ov2640_SCCB_Init(void)
{
    rcu_periph_clock_enable(RCU_OV2640_SCL);
    rcu_periph_clock_enable(RCU_OV2640_SDA);

    //初始化SCL
    gpio_mode_set(PORT_OV2640_SCL, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_OV2640_SCL);
    gpio_output_options_set(PORT_OV2640_SCL, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_OV2640_SCL);
    //初始化SDA
    gpio_mode_set(PORT_OV2640_SDA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_OV2640_SDA);
    gpio_output_options_set(PORT_OV2640_SDA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_OV2640_SDA);

    //SCL和SDA输出高电平
    gpio_bit_write(PORT_OV2640_SCL, GPIO_OV2640_SCL, SET);
    gpio_bit_write(PORT_OV2640_SDA, GPIO_OV2640_SDA, SET);

    OV2640_SDA_MODE_OUT(); //设置SDA为输出模式
}

/************************************************
 * @brief : IIC起始时序
 * @date : 2024-05-09
*************************************************/
void IIC_Start(void)
{
	OV2640_SDA_MODE_OUT();
	
	SDA(1);
    SCL(1); 
	delay_1us(50);  
	SDA(0);
	delay_1us(50);    
	SCL(0);	               
}

/************************************************
 * @brief : IIC停止时序
 * @date : 2024-05-09
*************************************************/
void IIC_Stop(void)
{
	OV2640_SDA_MODE_OUT();
	SDA(0);
	delay_1us(50);
	SCL(1);
	delay_1us(50);
	SDA(1);
	delay_1us(50);
	
}

/************************************************
 * @brief : 主机发送非应答信号
 * @date : 2024-05-09
*************************************************/
void IIC_Send_No_Ack(void)
{
	OV2640_SDA_MODE_OUT();

	delay_1us(50);
    SDA(1);
	SCL(1);
	delay_1us(50);
	SCL(0);
    delay_1us(50);
	SDA(0);
    delay_1us(50);
}

/************************************************
 * @brief : 等待从机应答
 * @return unsigned char    0，有应答；1，无应答    
 * @date : 2024-05-09
*************************************************/
unsigned char I2C_WaitAck(void)
{
	
	char ack = 0;
	unsigned char ack_flag = 10;
	SCL(0);
	SDA(1);
	OV2640_SDA_MODE_IN();
	delay_1us(5);
	SCL(1);
    delay_1us(5);

	while( (SDA_GET()==1) && ( ack_flag ) )
	{
		ack_flag--;
		delay_1us(5);
	}
	
	if( ack_flag <= 0 )
	{
		IIC_Stop();
		return 1;
	}
	else
	{
		SCL(0);
		OV2640_SDA_MODE_OUT();
	}
	return ack;
}

/************************************************
 * @brief : 写入一个字节
 * @param  dat   要写入的数据           
 * @return uint8_t    1，发送失败；0，发送成功      
 * @date : 2024-05-09
*************************************************/
uint8_t Send_Byte(uint8_t dat)
{
	uint8_t i = 0, res = 0;
	OV2640_SDA_MODE_OUT();
	SCL(0);//拉低时钟开始数据传输
	
	for( i = 0; i < 8; i++ )
	{
		SDA( (dat & 0x80) >> 7 );
        dat<<=1;
		delay_1us(50);
		SCL(1);
		delay_1us(50);
		SCL(0);
	}	
	OV2640_SDA_MODE_IN();		//设置SDA为输入
	delay_1us(50);
	SCL(1);			//接收第九位，以判断是否发送成功
	delay_1us(50);
	if( SDA_GET() )res=1;  //SDA=1发送失败
	else res=0;         //SDA=0发送成功
	SCL(0);		 
	OV2640_SDA_MODE_OUT();		//设置SDA为输出  
	return res;  
}

/************************************************
 * @brief : IIC读取时序
 * @return unsigned char    读到的数据
 * @date : 2024-05-09
*************************************************/
unsigned char Read_Byte(void)
{
	unsigned char i,receive=0;
	OV2640_SDA_MODE_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        delay_1us(50);
		SCL(1);
		receive=receive<<1;
		if( SDA_GET() )receive++;   
		delay_1us(50);
		SCL(0);    
    }					 
	OV2640_SDA_MODE_OUT();//SDA设置为输出 
    return receive;
}

/************************************************
 * @brief : 写寄存器
 * @param  reg      寄存器地址          
 * @param  data     要写入的值
 * @return uint8_t  0，成功；1，失败         
 * @date : 2024-05-09
*************************************************/
uint8_t IIC_WR_Reg(uint8_t reg,uint8_t data)
{
	uint8_t res=0;
	IIC_Start(); 					//启动IIC传输
	if( Send_Byte(IIC_ADDR))res=1;	//写器件ID 
	delay_1us(100);
  	if( Send_Byte(reg) )    res=2;  //写寄存器地址 
	delay_1us(100);
  	if( Send_Byte(data) )res=3; 	//写数据 
  	IIC_Stop();	  
  	return	res;
}

/************************************************
 * @brief : 读寄存器
 * @param  reg        寄存器地址     
 * @return uint8_t    读到的寄存器的值      
 * @date : 2024-05-09
*************************************************/
uint8_t IIC_RD_Reg(uint8_t reg)
{
	uint8_t val=0;
	IIC_Start(); 				//启动SCCB传输
	Send_Byte(IIC_ADDR);		//写器件ID	  
	delay_1us(100);	 
  	Send_Byte(reg);			//写寄存器地址	  
	delay_1us(100);	  
	IIC_Stop();   
	delay_1us(100);	   
	//设置寄存器地址后，才是读
	IIC_Start();
	Send_Byte(IIC_ADDR|0X01);	//发送读命令	  
	delay_1us(100);
  	val=Read_Byte();		 	//读取数据
  	IIC_Send_No_Ack();
  	IIC_Stop();
  	return val;
}