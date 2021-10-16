#include "iic.h"
#include "main.h"
#include "delay.h"
#include "led.h"
#include "usart.h"

int main(void)
{
    int i;
    delay_init();
    Led_Init();
    //delay_ms(3000);
    for(i = 0; i < 5; i++)
    {
        Led_On();
        delay_ms(100);
        Led_Off();
        delay_ms(100);
    }
		Usart_Init();
		//delay_ms(1000);
    IIC_Init();
    while (1)
    {
    }
}
