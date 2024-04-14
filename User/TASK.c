#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "PWM.h"
#include "PID_Control.h"
#include "vl53l0x.h"
#include "vl53l0x_gen.h"
#include "usart.h"
extern float Exp_Position;
extern float Set_PWM;
extern uint16_t Distance_data_last;
static char buf[VL53L0X_MAX_STRING_LENGTH];//����ģʽ�ַ����ַ�������
VL53L0X_Error Status=VL53L0X_ERROR_NONE;//����״̬

void Task_1(void)
{
	Exp_Position = 90;
	PID_Incremental_Init(&Position_INC, -1.6, -0.018, 0.38,2000, 0);//�����߷ֶθ�Kd
	while(1)
	{
		if(Task_Delay >1001)
		{
			Task_Delay = 0;
			break;
		}
		Status = vl53l0x_start_single_test(&vl53l0x_dev,&vl53l0x_data,buf);//ִ��һ�β���
		if(Status==VL53L0X_ERROR_NONE)
		{
			Distance_data = Distance_data;
			Distance_data -= 250;            //B����Ϊ������㣬�ټ���2cm��ƹ����뾶
			Distance_data = Distance_data*0.3 + Distance_data_last*0.7;
			Distance_data_last = Distance_data;		
			
		}

			printf("Measurement is Error!!!\r\n");	
		PID_Incremental_Calc(&Position_INC,Exp_Position,Distance_data);
        Set_PWM = Position_INC.out + 1200;
		PWM_SetCompare1(Set_PWM);
	}

	PID_Incremental_Init(&Position_INC, 0, 0, 0,2000, 0);
	PWM_SetCompare1(0);
	TIM_Cmd(TIM3, DISABLE);  //ʹ��TIMx		
}
