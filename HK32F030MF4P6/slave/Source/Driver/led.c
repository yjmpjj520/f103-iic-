#include "led.h"

void Led_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Schmit = GPIO_Schmit_Disable;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

void Led_On(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

void Led_Off(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
}




