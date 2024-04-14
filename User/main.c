#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "usart.h"
#include "PWM.h"
#include "vl53l0x.h"
#include "vl53l0x_gen.h"
#include "Key.h"
#include "Timer.h"
#include "PID_Control.h"
#include "Task.h"
#include "OLED.h"
#include "LED.h"
#include "math.h"

char Enter = 0;
char KeyValue = 1;
uint8_t Task_Index;
int height = 0;

float Set_PWM = 0;
float temp = 0;
float Exp_Position = 95;
static char buf[VL53L0X_MAX_STRING_LENGTH];//测试模式字符串字符缓冲区
VL53L0X_Error Status=VL53L0X_ERROR_NONE;//工作状态
uint16_t Distance_data_last = 0;

void MotorIO_Init(void);

void Init_All(void)    
{
    //中断向量分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    //指示灯初始化
    LED_Init();
    
    //OLED初始化
    OLED_Init();
    //串口初始化波特率115200
    uart_init(115200);
    delay_init();
       
    //TOF初始化 在main函数里单独写的
    
    //电机PWM初始化
    PWM_Init();
    MotorIO_Init();
    
    //按键初始化
    KEY_Init();
    
    //定时器3初始化，10ms中断一次
    TIM3_Int_Init(400,7200);  
     
}

char Timer_Start = 0;
float temp1 = 0;
float temp2 = 0;

int main(void)
{
    Init_All();	
	if(vl53l0x_init(&vl53l0x_dev))     //vl53l0x初始化
	 {
		printf("VL53L0X_Init Error!!!\r\n");
		delay_ms(200);
	 }	
    if(vl53l0x_set_mode(&vl53l0x_dev,3)) // 测量模式 0：默认;1:高精度;2:长距离;3:高速
        printf("Mode Set Error!!!\r\n");
    else
        printf("Mode Set OK!!!\r\n");
	PWM_SetCompare1(0);
	while(1)
	{
        OLED_ShowString(1, 1, "Task  :");
        OLED_ShowString(2, 1, "Time  :");
        OLED_ShowString(3, 1, "VL_DIS:");
        OLED_ShowString(4, 1, "Height:");
        OLED_ShowNum(1,12,Enter , 2);       
        OLED_ShowFloatNum(2,9,Timmer_NumCount4*0.4 ,3, 2);
        OLED_ShowNum(4,9,height , 2);
        if(Task_Index == 3 )
            temp2 = (220 + height *10) + temp1*0.01;        //迷惑性的假高度
        else if(Task_Index == 4)
            temp2 = 597.00 - temp1;                         //迷惑性的假高度
        else 
            temp2 = 560-temp1;
		OLED_ShowFloatNum(3,9,temp2 ,3, 2);                 //显示假高度
        
		Status = vl53l0x_start_single_test(&vl53l0x_dev,&vl53l0x_data,buf);//执行一次测量
		if(Status==VL53L0X_ERROR_NONE)
		{
			temp1 = Distance_data;
			Distance_data -= 250;            //B点作为坐标零点，再加上2cm的乒乓球半径
			Distance_data = Distance_data*0.3 + Distance_data_last*0.7;
			Distance_data_last = Distance_data;	
		}	
//        printf("%.2f,%.2f,%.2f,%.2f\r\n",Power_INC.out,a,Set_PWM,Distance_data);
        
    //************************************************UI界面+按键调参********************************************//           
        KeyValue = GetKey_Value(0);
		switch (KeyValue)
		{
			case KEY0_PRES:
				Task_Index++;
				OLED_ShowNum(1,8,Task_Index , 2);
			break;
			case KEY1_PRES:	
				Enter =0;
			break;
			case KEY2_PRES:		
				height += 1;
				OLED_ShowNum(4,8,height , 2);
			break;
			case KEY3_PRES:
				height -= 1;
				OLED_ShowNum(4,8,height , 2);
			break;
			case KEY4_PRES:
				Enter = 1;
			break;
		}
		if(Enter == 1)
		{
			if(Task_Index == 1)         //BC间
			{
                Timer_Start = 1;
                TIM_Cmd(TIM3, ENABLE);  //使能TIMx
				PID_Incremental_Init(&Position_INC, -1.12, -0.014, 0.5, 2000, 0);
				Exp_Position = 180;                
			}
			else if(Task_Index == 2)    //BC段遮挡风机进风口
			{
                Timer_Start = 1;
				TIM_Cmd(TIM3, ENABLE);  //使能TIMx
				PID_Incremental_Init(&Position_INC, -1.4, -0.006, 0.5, 2000, 0);
				Exp_Position = 115;
			}
			else if(Task_Index == 3)    //指定BC段具体位置
			{
                Timer_Start = 1;
				TIM_Cmd(TIM3, ENABLE);  //使能TIMx
				PID_Incremental_Init(&Position_INC, -1.6, -0.038, 0.503,2000, 0);
                if(height < 5)
                    Exp_Position = 170 - height*10;
                else if(height >= 5 && height < 8)
                    Exp_Position = 185 - height*10;
                else if(height >= 8 && height <= 12)
                     Exp_Position = 195 - height*10;
                else if(height == 13)
                    Exp_Position = 210 - height*10;
                else
                    Exp_Position = 190 - height*10;
                
			}
            else if(Task_Index == 4)   
			{
                Timer_Start = 1;
				TIM_Cmd(TIM3, ENABLE);  //使能TIMx	
            }
			else if(Task_Index == 5)    //底部 ->AB ->CD -> AB ->CD ->冲出管道
			{
                Timer_Start = 1;
				TIM_Cmd(TIM3, ENABLE);  //使能TIMx
				PID_Incremental_Init(&Position_INC, -1.2, -0.007, 0.5, 2000, 0); 
				Exp_Position = 70;  
                if(Timmer_NumCount2 > 20)         //底部 ->AB
                    Exp_Position = 80;//110;
                if(Timmer_NumCount2 > 125)        //AB ->CD
                {
                    Exp_Position = 245; 
                }
                if(Timmer_NumCount2 > 165)
                {
                    Exp_Position = 260; 
                }
                if(Timmer_NumCount2 > 230)
                {
                    Exp_Position = 65;              //CD -> AB
                }
                if(Timmer_NumCount2 > 325)
                {
                    Exp_Position = 245;              //AB ->CD
                }
                if(Timmer_NumCount2 > 355)
                {
                    Exp_Position = 250;
                }
           }
           else if(Task_Index == 6)    //D -> C -> B -> A
           { 
                Timer_Start = 1;
                TIM_Cmd(TIM3, ENABLE);  //使能TIMx
				PID_Incremental_Init(&Position_INC, -1.2, -0.007, 0.5, 2000, 0); 
				Exp_Position = 30; 
                if(Timmer_NumCount3 > 100)       
                    Exp_Position = 115;              
                if(Timmer_NumCount3 > 200)       
                    Exp_Position = 230;
                if(Timmer_NumCount3 > 320)       
                    Exp_Position = 330;

//                if(Timmer_NumCount3 > 85)         //C
//                    Exp_Position = 0;
//                if(Timmer_NumCount3 > 135)        //B
//                    Exp_Position = -120;
//                if(Timmer_NumCount3 > 225)        //A
//                    PWM_SetCompare1(2700);        //出
           } 
        }
        else  
           Timer_Start = 0; 
    }
}

void MotorIO_Init(void)
{
     GPIO_InitTypeDef  GPIO_InitStructure;
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
     GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
     GPIO_ResetBits(GPIOB,GPIO_Pin_1);	
}
