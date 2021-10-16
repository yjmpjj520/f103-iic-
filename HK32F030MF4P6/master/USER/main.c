//#include "timer.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "iic.h"


extern u8 buff[10];

int main(void)
{
    u8 byte = 10;
    u8 i;
    u8 p = 0;
    delay_init();
    uart_init(115200);	 //串口初始化为115200
    IIC_Init();
    delay_ms(1000);
    //TIMER3_Init(5000-1,7200-1);
    printf("\r\nSystem running...");

    while(1)
    {
        // srand(time_us);
        //  p= rand()%100 ;      //产生一个30到50的随机数
        // printf("p=%d\r\n ",p);


        //写操作
        IIC_Start();
        IIC_Send_Byte(0x20);        //发送器件地址
        while( IIC_Wait_Ack());         //等待应答
        IIC_Send_Byte(0x11);
        while( IIC_Wait_Ack());
        IIC_Send_Byte(0x02);
        while( IIC_Wait_Ack());
        IIC_Send_Byte(0x03);
        while( IIC_Wait_Ack());
        IIC_Send_Byte(0x04);
        while( IIC_Wait_Ack());
        IIC_Send_Byte(0x05);
        while( IIC_Wait_Ack());
        IIC_Send_Byte(0x06);
        while( IIC_Wait_Ack());
        IIC_Send_Byte(0x07);
        while( IIC_Wait_Ack());
        IIC_Send_Byte(0x08);
        while( IIC_Wait_Ack());
        IIC_Send_Byte(0x09);
        while( IIC_Wait_Ack());
        IIC_Send_Byte(0x10);
        while( IIC_Wait_Ack());
        IIC_Stop();
        delay_ms(10);

        //读操作
        IIC_Start();
        IIC_Send_Byte(0x21);

        while(IIC_Wait_Ack());
        for(i = 0; i < byte; i++)      //连续读十个字节
        {
            buff[i] = IIC_Read_Byte(1);
        }
        buff[byte - 1] =  IIC_Read_Byte(0);
        IIC_Stop();
        delay_ms(100);

        for(i = 0; i < byte; i++)
        {
            printf("\r\nbuff=%d", buff[i]);  //打印所读数据
        }
    }
}

