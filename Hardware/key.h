#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


#define KEY0    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)//��ȡ����0
#define KEY1    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)//��ȡ����1
#define KEY2    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//��ȡ����2 
#define KEY3    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)//��ȡ����3
#define KEY4    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)//��ȡ����4

 

#define KEY0_PRES 	1	//KEY0����
#define KEY1_PRES	2	//KEY1����
#define KEY2_PRES	3	//KEY2����
#define KEY3_PRES   4	//KEY3����
#define KEY4_PRES   5	//KEY4����

int filter(int value);
void KEY_Init(void);    //IO��ʼ��
uint8_t GetKey_Value(uint8_t mode);  	    //����ɨ�躯��			

#endif
