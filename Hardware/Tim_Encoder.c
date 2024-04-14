#include "stm32f10x.h"                  // Device header

void TimEncoder_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStucture;
	GPIO_InitStucture.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStucture.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStucture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStucture);	
	
	//TIM_InternalClockConfig(TIM3); �������ӿ������ź���Ϊ�ⲿʱ��
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	// �ߵ͵�ƽ���Բ���ת ���������ӿ����۸ߵ͵�ƽ����Ч
	// TIM_ICPolarity_Falling ����
	//������֮��ı�����ģʽ������
	//TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF; 
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,
						TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM3,ENABLE);
}

int16_t TimEncoder_Get()
{
	int16_t Temp1=0;
	Temp1 = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);
	return Temp1;
}

//void TIM3_IRQHandler(void)  //0.2s����һ���ж�
//{
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)  //�鿴TIM2�ĸ����жϱ�־λ
//	{
//		//speed = TimEncoder_Get();
//		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //�ֶ������־λ
//	}
//}
