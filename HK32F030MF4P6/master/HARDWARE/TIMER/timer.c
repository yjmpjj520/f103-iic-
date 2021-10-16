#include "timer.h"
#include "iic.h"
#include "usart.h"
#include "time.h"
#include "stdlib.h"
#include "delay.h"

void TIMER3_Init(u16 arr,u8 psc){
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}

extern u8 buff[128]={0};
extern u32 time_us=0;
u8 p=0;
u8 byte=1;
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	      u8 i=0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
        time_us++;
			  srand(time_us);
        p= rand()%100 ;      //产生一个100以内的随机数
        printf("p=%d\r\n ",p);
			
			    //写操作
        IIC_Start();
        IIC_Send_Byte(0x20);        //发送器件地址
        while( IIC_Wait_Ack());         //等待应答
        IIC_Send_Byte(p);        
        while( IIC_Wait_Ack());       
        IIC_Stop();
        delay_ms(10);

        //读操作
        IIC_Start();
        IIC_Send_Byte(0x21);
        while(IIC_Wait_Ack());         
        for(i = 0; i < byte; i++)      //连续读十个字节
        {
            buff[i] = IIC_Read_Byte(0);
        }
       // buff[byte-1] =  IIC_Read_Byte(0);
        IIC_Stop();
        delay_ms(10);
			
				for(i=0;i<byte;i++){
				printf(" buff=%d\r\n", buff[i]);  //打印所读数据
				}
			
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
}

