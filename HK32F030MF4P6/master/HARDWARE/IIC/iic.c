
#include "IIC.h"
#include "delay.h"

//��ʼ������ gpio
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ�� GPIOA ʱ��


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��

    I2C_SDA_H;
    I2C_SCL_H;

}

//����IIC��ʼ�ź�:SCKΪ��ʱ��SCLһ���½��ء�
void IIC_Start(void)
{
    I2C_SDA_H;
    I2C_SCL_H;
    delay_us(4);
    I2C_SDA_L;
    delay_us(4);
    I2C_SCL_L;//ǯסI2C���ߣ�׼�����ͻ��������
}


//����IICֹͣ�ź�:SCKΪ��ʱ��SCLһ��������
void IIC_Stop(void)
{
    I2C_SCL_L;
    I2C_SDA_L;
    delay_us(4);
    I2C_SCL_H;
    I2C_SDA_H;
    delay_us(4);
}


//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
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

    I2C_SCL_L;//ʱ�����0
    return 0;
}

//����ACKӦ��
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
//������ACKӦ��
void IIC_NAck(void)
{
    I2C_SCL_L;
    I2C_SDA_H;
    delay_us(2);
    I2C_SCL_H;
    delay_us(2);
    I2C_SCL_L;
}

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    I2C_SCL_L;//����ʱ�ӿ�ʼ���ݴ���

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

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK

    return receive;
}









