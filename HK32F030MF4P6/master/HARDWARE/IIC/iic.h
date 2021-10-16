
#ifndef __IIC_H
#define __IIC_H 	
#include "sys.h"

//PA7:I2C2_SCL    PA5:I2C2_SDA
//���ģ��IIC

#define I2C_SCL_H GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define I2C_SCL_L GPIO_ResetBits(GPIOA,GPIO_Pin_7)//Ƭѡ�źţ�����Ч�� 

#define I2C_SDA_H GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define I2C_SDA_L GPIO_ResetBits(GPIOA,GPIO_Pin_5)//���ݿ�

#define REDAI2C_SDA GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define REDAI2C_SCL GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(unsigned char ack);

#endif

