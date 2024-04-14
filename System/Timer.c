#include "stm32f10x.h"                  // Device header  
#include "stdio.h"
#include "vl53l0x.h"
#include "vl53l0x_gen.h"
#include "LED.h"
#include "PWM.h"
#include "PID_Control.h"

extern uint8_t Task_Index;
extern char Enter;
extern float Exp_Position;
extern float Set_PWM;
float Exp_Pos;

uint8_t  Timmer_NumCount1 = 0;
uint32_t Timmer_NumCount2 = 0;
uint32_t Timmer_NumCount3 = 0;
uint32_t Timmer_NumCount4 = 0;
uint8_t x10_ms = 0;
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
 
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
 
	TIM_Cmd(TIM3, DISABLE);  //ʹ��TIMx					 
}

extern char Timer_Start;
uint32_t Timer_Stop1 = 0;
uint32_t Timer_Stop2 = 0;
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
    {
        Timmer_NumCount1++;
        if(Task_Index == 5)            
            Timmer_NumCount2++;
        if(Task_Index == 6)   
            Timmer_NumCount3++;
        Timmer_NumCount4++;
        if(Timer_Start == 0)
            Timmer_NumCount4 = 0;
        if(Timer_Stop1 == 1)
            Timmer_NumCount2 = 0;
        if(Timer_Stop2 == 1)
            Timmer_NumCount3 = 0;
		PID_Incremental_Calc(&Position_INC,Exp_Position,Distance_data);
		Set_PWM = Position_INC.out + 1200;
		
		if(Enter == 1)
			PWM_SetCompare1(Set_PWM);
		else
			PWM_SetCompare1(0);	
		if(Task_Index == 4 &&  Enter == 1)
			PWM_SetCompare1(1390);	
        
         if(Timmer_NumCount2 > 440 && Task_Index == 5 && Enter == 1)          //CD ->����ܵ�
            PWM_SetCompare1(2700);	
         if(Timmer_NumCount2 >= 465)
             Timer_Stop1 = 1;
         
          if(Timmer_NumCount3 > 400 && Task_Index == 6 && Enter == 1)          //CD ->����ܵ�
            PWM_SetCompare1(2700);	
         if(Timmer_NumCount3 >= 420)
             Timer_Stop2 = 1;
            
        
		printf("%.2f\r\n",Set_PWM);
        //����,�жϳ����Ƿ�����
        if(Timmer_NumCount1 > 50)
        {
            LED0 = !LED0;
            Timmer_NumCount1 = 0;
        }  
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update );  //���TIMx�����жϱ�־ 
    }
}








