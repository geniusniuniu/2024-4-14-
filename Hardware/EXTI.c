#include "stm32f10x.h"                  // Device header          


uint16_t ItCount = 0;
void EXTi_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //���գ��������������붼��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//����AFIO����
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14); //ѡ��GPIOB��14��������Ϊ�ⲿ�ж���
	
	//��ʼ��EXTI����
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14; //�ж���Դ
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;  //ʹ��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
	
	EXTI_Init(&EXTI_InitStructure);
	
	//����NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);
}

uint16_t Get_ItCount(void)
{
	return ItCount;
}


void EXTI15_10_IRQHandler(void) //�жϺ��������޲��޷���ֵ��
{
	//�Ƚ����жϱ�־λ���ж�
	if(EXTI_GetITStatus(EXTI_Line14)==SET)
	{
		
		ItCount++;
		
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}
	
