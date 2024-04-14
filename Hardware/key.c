#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
    GPIO_InitTypeDef GPIO_InitStruct;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);
    
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
 	GPIO_Init(GPIOB,&GPIO_InitStruct);
    
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //PA0���ó����룬Ĭ������	
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  
    
	GPIO_Init(GPIOA, &GPIO_InitStruct);//��ʼ��GPIOA.0


}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
uint8_t GetKey_Value(uint8_t mode)
{	 
	static uint8_t key_up=1;//�������ɿ���־
	if(mode)
        key_up=1;       //֧������		  
	if(key_up && (KEY0==0||KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
        if      (KEY0==0)   return KEY0_PRES;
		else if (KEY1==0)   return KEY1_PRES;
		else if (KEY2==0)   return KEY2_PRES;
		else if (KEY3==0)   return KEY3_PRES;
        else if (KEY4==0)   return KEY4_PRES;
	}
    else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)
        key_up=1; 	    
 	return 0;// �ް�������
}



#define WINDOW_SIZE 5

int window[WINDOW_SIZE];
int windowIndex = 0;

int filter(int value) 
{
    static int sum = 0;
    
    // ���»�������
    sum = sum - window[windowIndex] + value;
    window[windowIndex] = value;
    windowIndex = (windowIndex + 1) % WINDOW_SIZE;
    
    // ���㴰����ƽ��ֵ
    int avg = sum / WINDOW_SIZE;
    
    return avg;
}

