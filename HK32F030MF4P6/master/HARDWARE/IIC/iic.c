
#include "IIC.h"
#include "delay.h"

//初始化引脚 gpio
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能 GPIOA 时钟


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

    I2C_SDA_H;
    I2C_SCL_H;

}

//产生IIC起始信号:SCK为高时，SCL一个下降沿。
void IIC_Start(void)
{
    I2C_SDA_H;
    I2C_SCL_H;
    delay_us(4);
    I2C_SDA_L;
    delay_us(4);
    I2C_SCL_L;//钳住I2C总线，准备发送或接收数据
}


//产生IIC停止信号:SCK为高时，SCL一个上升沿
void IIC_Stop(void)
{
    I2C_SCL_L;
    I2C_SDA_L;
    delay_us(4);
    I2C_SCL_H;
    I2C_SDA_H;
    delay_us(4);
}


//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    I2C_SDA_H;
    delay_us(1);
    I2C_SCL_H;
    delay_us(1);

    while(REDAI2C_SDA)
    {
        ucErrTime++;

        if(ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }

    I2C_SCL_L;//时钟输出0
    return 0;
}

//产生ACK应答
void IIC_Ack(void)
{
    I2C_SCL_L;
    I2C_SDA_L;
    delay_us(2);
    I2C_SCL_H;
    delay_us(2);
    I2C_SCL_L;
    I2C_SDA_H;
}
//不产生ACK应答
void IIC_NAck(void)
{
    I2C_SCL_L;
    I2C_SDA_H;
    delay_us(2);
    I2C_SCL_H;
    delay_us(2);
    I2C_SCL_L;
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    I2C_SCL_L;//拉低时钟开始数据传输

    for(t = 0; t < 8; t++)
    {
        if((txd & 0x80) >> 7)
        {
            I2C_SDA_H;
        }
        else
        {
            I2C_SDA_L;
        }

        txd <<= 1;
        delay_us(2);
        I2C_SCL_H;
        delay_us(2);
        I2C_SCL_L;
        delay_us(2);
    }
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;

    for(i = 0; i < 8; i++ )
    {
        I2C_SCL_L;
        delay_us(2);
        I2C_SCL_H;
        receive <<= 1;

        if(REDAI2C_SDA)
        {
            receive++;
        }

        delay_us(1);
    }

    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK

    return receive;
}









