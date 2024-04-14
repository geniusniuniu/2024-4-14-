#include "game.h"
#include "oled.h"
#include "gpio.h"
#include "main.h"

// 电赛任务框架 带注释部分需自行更改

//这里改为自己的按键引脚
#define  turn_key  P1.0
#define  start_key P3.5


int mode_num = 0;
int start_flag = 1;
int keep_time = 0;
int tof_num=0;
int pwmvalue=0;
float kd=0;
float ki=0;
float kp=0;

void oled_show_game_static();

void base_init(void)//这里加入自己的初始化
{
//tof初始化

//motor_pwm初始化
 HAL_TIM_PWM_Init(&htim3);
 HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
 __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
//pid初始化
 pid_Init();
//oled初始化
 OLED_Init();
 oled_show_game_static();
//key初始化  至少两个


//把 oled_show_game() 放到定时器中断里


}


void key_game()
{
	if(turn_key)//这里根据自己获取GPIO电平的函数，以下部分自行修改
	{
		HAL_Delay(100);//按照自己的delay函数修改，以下部分自行修改
		mode_num++;
		if(mode_num>=4)
		{
			mode_num =0;
		}
	}
	if(start)
	{
		HAL_Delay(100);
		start_flag = ~ start_flag;
		keep_time = 0;
	}
}


int key_num;
void key_set_num()
{
	if(turn_key)
	{
		HAL_Delay(10);
		key_num++;
		//这里可以自行优化
		if(key_num>=100)
		{
			key_num =0;
		}
	}
	if(start_key)
	{
		HAL_Delay(10);
		start_flag = ~ start_flag;
	}
}

void game_state()
{
	key_game();

	switch(mode_num)
	{
	case 1:
		while(start_flag)
		{
//=================================在这里加任务1的代码

//=================================
			if(start_flag == 0)
			{
				break;
			}
		}
		;break;
	case 2:
		while(start_flag)
		{
//=================================add your code

//=================================
			if(start_flag == 0)
			{
				break;
			}
		}
		break;
	case 3:
		key_set_num();
		while(start_flag)
		{
//=================================add your code

//=================================
			if(start_flag == 0)
			{
				break;
			}
		}
		break;
	case 4:
		while(start_flag)
		{
//=================================add your code

//=================================
			if(start_flag == 0)
			{
				break;
			}
		}
		break;
	}

}

//静态部分
void oled_show_game_static()     //将这里的显示函数修改成自己的函数
{
	OLED_PrintASCIIString(9,0*8,"mode",afont8x6,0);
	OLED_PrintASCIIString(9,1*8,"start",afont8x6,0);
	OLED_PrintASCIIString(9,2*8,"TOF",afont8x6,0);
	OLED_PrintASCIIString(9,3*8,"TIME",afont8x6,0);
	OLED_PrintASCIIString(9,4*8,"PWM",afont8x6,0);
	OLED_PrintASCIIString(9,5*8,"KP",afont8x6,0);
	OLED_PrintASCIIString(9,6*8,"KI",afont8x6,0);
	OLED_PrintASCIIString(9,7*8,"KD",afont8x6,0);

	OLED_PrintASCIIChar(63,0*8,'=',afont8x6,0);
	OLED_PrintASCIIChar(63,1*8,'=',afont8x6,0);
	OLED_PrintASCIIChar(63,2*8,'=',afont8x6,0);
	OLED_PrintASCIIChar(63,3*8,'=',afont8x6,0);
	OLED_PrintASCIIChar(63,4*8,'=',afont8x6,0);
	OLED_PrintASCIIChar(63,5*8,'=',afont8x6,0);
	OLED_PrintASCIIChar(63,6*8,'=',afont8x6,0);
	OLED_PrintASCIIChar(63,7*8,'=',afont8x6,0);
	OLED_ShowFrame();

}

//动态部分
void oled_show_game()//将这里的显示函数修改成自己的oled函数
{
	//动态显示
	OLED_NewFrame();
	OLED_PrintNum(72,0*8, mode_num,0);//这里改成自己的响应的函数名
	OLED_PrintNum(72,1*8, start_flag,0);
	OLED_PrintNum(72,2*8, tof_num,0);
	OLED_PrintNum(72,3*8, keep_time,0);
	OLED_PrintNum(72,4*8,pwmvalue ,0);
	OLED_PrintFloatNum(72,5*8, kp,0);
	OLED_PrintFloatNum(72,6*8, ki,0);
	OLED_PrintFloatNum(72,7*8, kd,0);
	OLED_ShowFrame_game();//刷新半边
}
