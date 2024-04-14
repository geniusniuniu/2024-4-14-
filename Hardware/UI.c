#include "ui.h"
#include <string.h>
#include <stdio.h>
#include "vl53l0x_gen.h"
#include "OLED.h"
#include "stdint.h"
#include "PID_Control.h"

extern float Set_PWM ;//保存测距数据
extern float b;
uint8_t page=0;
uint8_t line=0;
float value[4][7];
char value_name[4][7][7];
///////////////////////////////////////////////////待显示参数




void value_name_init(void)//将name改为想要的参数名，但“ ”中的总长度不要变
{
	//page=0
	strcpy(value_name[0][0],"VL_DIS"); //NAME_1表示显示的是第一页
	strcpy(value_name[0][1],"SetPWM");
	strcpy(value_name[0][2],"Time  ");
	strcpy(value_name[0][3],"Task  ");


	//page=1	
	strcpy(value_name[1][0],"Task1 ");
	strcpy(value_name[1][1],"Ang_Ed");
	strcpy(value_name[1][2],"T_Kd  ");
	strcpy(value_name[1][3],"Exp_Sp");

	
	//page=2
	strcpy(value_name[2][0],"Sp_Kp3");
	strcpy(value_name[2][1],"Sp_Ki ");
	strcpy(value_name[2][2],"Circle");
	strcpy(value_name[2][3],"Vl_Dis");
	
	//page=3
	strcpy(value_name[3][0],"Ratio ");
	strcpy(value_name[3][1],"Dis1  ");
	strcpy(value_name[3][2],"Dis2  ");
	strcpy(value_name[3][3],"An_Sum");

}

void refresh_value()//value=填入变量名
{
	//page=0
    value[0][0]= Distance_data;
	value[0][1]= Set_PWM;
	value[0][2]= 0.0;
	value[0][3]= 0.0;
	
	//page=1
	value[1][0]= 0.0;
	value[1][1]= 0.0;
	value[1][2]= 0.0;
	value[1][3]= 0.0;
	
	//page=2
	value[2][0]= 0.0;
	value[2][1]= 0.0;
	value[2][2]= 0.0;
	value[2][3]= 0.0;
	
	//page=3
	value[3][0]= 0.0;
	value[3][1]= 0.0;//Sum_Dis1;
	value[3][2]= 0.0;//Sum_Dis2;
	value[3][3]= 0.0;//Sum_Angle_C1;
		
}

void oled_all_static_state()  //静态显示部分
{
    uint8_t j;
	for(j=0;j<=3;j++)
	{
        OLED_ShowString(j+1, 1, value_name[page][j]);
		OLED_ShowString(j+1, 7, "=");
	}
}


void oled_show(void)
{
    uint8_t j;
	refresh_value();
	for(j=0;j<=3;j++)
	{
        OLED_ShowFloatNum(j+1,8,value[page][j] , 4, 2);
	}
}

//////////////////////////////////////////////////////////////
void ui_init(void)
{
	OLED_Init();
	value_name_init();
	oled_all_static_state();
}

void ui_show(void)
{
	refresh_value();
	oled_show();
}



