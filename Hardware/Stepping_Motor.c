#include "stm32f10x.h"
#include "stepping_motor.h"
#include "delay.h"

/*
ʹ�ö�ʱ����ת����
Timer3
PA6->DIR;PA5->PWM;
Timer2
PA4->DIR;PA3->PWM;
*/


void SteppingMotor_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_6 |GPIO_Pin_5 | GPIO_Pin_4| GPIO_Pin_3);	

}


//ʹ��whileѭ����תGPIO��
//GPIO_Pin_3 ->  Dir
//GPIO_Pin_4 ->  Pulse
//����Ȧ�����ٶȣ�ϸ������
void SteppingMotor_Run(u8 Dir, u8 Circle_Num, u16 Speed, int Micro_Step)
{
    int i = 0;
    int Circle_Pulse = 720/(1.8/Micro_Step);    //��ƽ��ת������һ������
    if(Dir == CW)
        GPIO_SetBits(GPIOA,GPIO_Pin_3);
    else
       GPIO_ResetBits(GPIOA,GPIO_Pin_3); 
    
    for(i=0; i<=(Circle_Num * Circle_Pulse); i++)
    {
        delay_us(Speed);        //ʱ����ԽС�ٶ�Խ�죿����������
        GPIOA->ODR ^= GPIO_Pin_4;  
    }
}




















