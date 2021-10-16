#include "timer.h"
#include "iic.h"
#include "usart.h"
#include "time.h"
#include "stdlib.h"
#include "delay.h"

void TIMER3_Init(u16 arr,u8 psc){
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}

extern u8 buff[128]={0};
extern u32 time_us=0;
u8 p=0;
u8 byte=1;
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	      u8 i=0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
        time_us++;
			  srand(time_us);
        p= rand()%100 ;      //����һ��100���ڵ������
        printf("p=%d\r\n ",p);
			
			    //д����
        IIC_Start();
        IIC_Send_Byte(0x20);        //����������ַ
        while( IIC_Wait_Ack());         //�ȴ�Ӧ��
        IIC_Send_Byte(p);        
        while( IIC_Wait_Ack());       
        IIC_Stop();
        delay_ms(10);

        //������
        IIC_Start();
        IIC_Send_Byte(0x21);
        while(IIC_Wait_Ack());         
        for(i = 0; i < byte; i++)      //������ʮ���ֽ�
        {
            buff[i] = IIC_Read_Byte(0);
        }
       // buff[byte-1] =  IIC_Read_Byte(0);
        IIC_Stop();
        delay_ms(10);
			
				for(i=0;i<byte;i++){
				printf(" buff=%d\r\n", buff[i]);  //��ӡ��������
				}
			
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
}

