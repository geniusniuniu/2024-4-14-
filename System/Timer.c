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
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
 
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
 
	TIM_Cmd(TIM3, DISABLE);  //使能TIMx					 
}

extern char Timer_Start;
uint32_t Timer_Stop1 = 0;
uint32_t Timer_Stop2 = 0;
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
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
        
         if(Timmer_NumCount2 > 440 && Task_Index == 5 && Enter == 1)          //CD ->冲出管道
            PWM_SetCompare1(2700);	
         if(Timmer_NumCount2 >= 465)
             Timer_Stop1 = 1;
         
          if(Timmer_NumCount3 > 400 && Task_Index == 6 && Enter == 1)          //CD ->冲出管道
            PWM_SetCompare1(2700);	
         if(Timmer_NumCount3 >= 420)
             Timer_Stop2 = 1;
            
        
		printf("%.2f\r\n",Set_PWM);
        //闪灯,判断程序是否运行
        if(Timmer_NumCount1 > 50)
        {
            LED0 = !LED0;
            Timmer_NumCount1 = 0;
        }  
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update );  //清除TIMx更新中断标志 
    }
}








