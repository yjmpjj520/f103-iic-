#include "iic.h"
#include "led.h"
#include "delay.h"
#include "usart.h"



void IIC_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef  I2C_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  //使能GPIOB
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_IOMUX, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);//GPIOA初始化

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_0);

    GPIOMUX->PIN_FUNC_SEL |= 0x00000004;

    RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); //使能I2C1

    //I2C_DeInit(I2C1);
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;//应答使能
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; //7位寻址
    I2C_InitStruct.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
    I2C_InitStruct.I2C_DigitalFilter = 0x00;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_Timing = 0x00B00000;
    I2C_InitStruct.I2C_OwnAddress1 = 0x20;    //自身地址
    I2C_StretchClockCmd(I2C1, DISABLE);
    I2C_SlaveByteControlCmd(I2C1, DISABLE);
    I2C_Init(I2C1, &I2C_InitStruct);  //I2C初始化

    I2C_ITConfig(I2C1, I2C_IT_ADDR | I2C_IT_TXI | I2C_IT_RXI | I2C_IT_STOPI, ENABLE); //使能地址匹配中断

    NVIC_InitStruct.NVIC_IRQChannel = I2C1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
    NVIC_Init(&NVIC_InitStruct);    //中断初始化

    I2C_Cmd(I2C1, ENABLE);

}

static uint8_t Rx_buffer[128] ;
static uint8_t Tx_count = 0;
static uint8_t Rx_count = 0;
static uint8_t Tx_MAX = 10;
static uint8_t Rx_MAX = 10;
static uint8_t i = 0;
static uint8_t h = 0;
//中断服务函数
void I2C1_IRQHandler(void)
{
    if(I2C_GetITStatus(I2C1, I2C_IT_TXIS) != 0)
    {
        I2C_ClearITPendingBit(I2C1, I2C_IT_TXIS);
        I2C_SendData(I2C1, Rx_buffer[Tx_count++]);

        if(Tx_count >= Tx_MAX)
        {
            Tx_count = 0;
        }
    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_RXNE) != 0)
    {
        Rx_buffer[Rx_count++] = I2C_ReceiveData(I2C1);
        I2C_ClearITPendingBit(I2C1, I2C_IT_RXNE);

        if(Rx_count >= Rx_MAX)
        {
            Rx_count = 0;
        }
    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_STOPF) != 0)
    {
        switch(i)
        {
            case 0:
                Rx_count = 0;
                Tx_count = 0;
                i++;
                I2C_ClearITPendingBit(I2C1, I2C_IT_STOPF);
                I2C_SendData(I2C1, Rx_buffer[Tx_count++]);
                break;

            case 1:
                I2C1->ISR |= I2C_ISR_TXE;
                I2C1->ISR |= I2C_ISR_TXIS;
                I2C_ClearITPendingBit(I2C1, I2C_IT_STOPF);
						    // I2C_SendData(I2C1, Rx_buffer[Tx_count++]);
                i = 0;
                break;
        }
    }
}
/*void I2C1_IRQHandler(void)
{
    if(I2C_GetITStatus(I2C1, I2C_IT_ADDRI) != 0)
    {

        I2C_ClearITPendingBit(I2C1, I2C_IT_ADDRI);//USART_SendData(USART1, 0x5a);
    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_TXIS) != 0)
    {
			  I2C_SendData(I2C1, Rx_buffer[Tx_count++]);
        I2C_ClearITPendingBit(I2C1, I2C_IT_TXIS);
        
       // I2C_SendData(I2C1, Rx_buffer[Tx_count++]);

        if(Tx_count >= Tx_MAX)
        {
            Tx_count = 0;
        }
    }

    else if(I2C_GetITStatus(I2C1, I2C_IT_RXNE) != 0)
    {
        Rx_buffer[Rx_count++] = I2C_ReceiveData(I2C1);
        I2C_ClearITPendingBit(I2C1, I2C_IT_RXNE);

        if(Rx_count >= Tx_MAX)
        {
            Rx_count = 0;
        }
    }

    else if(I2C_GetITStatus(I2C1, I2C_IT_STOPF) != 0)
    {
        Rx_count = 0;
        Tx_count = 0;
        I2C1->ISR |= I2C_ISR_TXE;
        I2C1->ISR |= I2C_ISR_TXIS;
        I2C_ClearITPendingBit(I2C1, I2C_IT_STOPF);
      //  I2C_SendData(I2C1, Rx_buffer[Tx_count++]);
    }
    else
    {
    }
}*/

