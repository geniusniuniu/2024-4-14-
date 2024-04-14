#include "stm32f10x.h"                  // Device header
#include "PID_Control.h"
#include "vl53l0x.h"
#include "vl53l0x_gen.h"

#define LOWPASS_FILTER	0.7
#define ERROR_MAX       100

//限幅函数
void Limit_Out(float *Output,float Limit_Min,float Limit_Max)
{
    if (*Output <= Limit_Min) *Output = Limit_Min;
    else if (*Output >= Limit_Max) *Output = Limit_Max;
}

float Num2Abs(float x)
{
	if(x < 0)
		return -x;
	return x;
}


//***********************************************************位置式PID************************************************************//
PID_InitTypeDef Power_POS;
PID_InitTypeDef Speed_POS;
PID_InitTypeDef Position_POS;

void PID_Init(PID_InitTypeDef *PID_Struct, float Kp, float Ki, float Kd,float Out_Limit, float Integral_Limit)		//PID初始化
{
	PID_Struct->Kp = Kp;
	PID_Struct->Ki = Ki;
	PID_Struct->Kd = Kd;
	
	PID_Struct->Err = 0;
	PID_Struct->Err_last = 0;
	
	PID_Struct->PID_Out = 0;
	PID_Struct->Out_Limit = Out_Limit;
	PID_Struct->Integral_Limit = Integral_Limit;
}


void PID_Calculate(PID_InitTypeDef *PID_Struct, float Exp_Val, float Act_Val)		//PID计算
{ 
	PID_Struct->Err = Exp_Val-Act_Val;		//err值为期望偏差与当前偏差的差值	
	PID_Struct->Integral += PID_Struct->Err;		//误差值累加	
	
	if(PID_Struct->Integral_Limit != 0)
	{
		//对积分低通滤波
		PID_Struct->Integral = PID_Struct->Integral_Last*LOWPASS_FILTER + PID_Struct->Integral*(1-LOWPASS_FILTER);
		
		PID_Struct->Integral_Last = PID_Struct->Integral;	//更新上一次积分
	}
	
	//积分限幅
	if(PID_Struct->Integral > PID_Struct->Integral_Limit)
		PID_Struct->Integral = PID_Struct->Integral_Limit;
	else if(PID_Struct->Integral < -PID_Struct->Integral_Limit)
		PID_Struct->Integral = -PID_Struct->Integral_Limit;
	
	PID_Struct->PID_Out = PID_Struct->Err * PID_Struct->Kp + 
								PID_Struct->Integral * PID_Struct->Ki +						//计算总输出量
									(PID_Struct->Err - PID_Struct->Err_last)*(PID_Struct->Kd)+1101;
	
	//输出限幅
	if(PID_Struct->PID_Out > PID_Struct->Out_Limit)
		PID_Struct->PID_Out = PID_Struct->Out_Limit; 

	
	PID_Struct->Err_last = PID_Struct->Err;				//更新上一次err
}




//***********************************************************增量式PID**************************************************//

PID_Incremental Power_INC;
PID_Incremental Position_INC;

void PID_Incremental_Init(PID_Incremental *pid, float Kp, float Ki, float Kd,float Out_Limit, uint8_t use_lowpass_filter)
{
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;
	
    pid->error = 0;
    pid->last_error = 0;
    pid->last_last_error = 0;
    pid->last_out = 0;
    pid->out = 0;
    pid->outmax = Out_Limit;
    pid->outmin = -Out_Limit;
    pid->use_lowpass_filter = use_lowpass_filter;
    pid->lowpass_filter_factor = 0.3;
}


float PID_Incremental_Calc(PID_Incremental *pid, float setpoint, float input_value)
{
	float derivative;
	float output_increment;
    pid->last_last_error = pid->last_error;
    pid->last_error = pid->error;
    pid->error = setpoint - input_value;
    derivative = (pid->error - 2 * pid->last_last_error + pid->last_last_error);
    output_increment = pid->Kp * (pid->error - pid->last_error) + pid->Ki * pid->error + pid->Kd * derivative;

    pid->out += output_increment;

    // Output limit
   	Limit_Out(&pid->out,pid->outmin,pid->outmax);							//输出限幅

    // Low pass filter
    if(pid->use_lowpass_filter){
        pid->out = pid->last_out * pid->lowpass_filter_factor + pid->out * (1 - pid->lowpass_filter_factor);
    }

    pid->last_out = pid->out;

    return pid->out;
}


float Actual_Speed = 0;
float Actual_Speed_last = 0;
float Error = 0;
float Pre_Dis = 0;
void PID_Control(void)
{
    static uint8_t Position_Time=0;
    static uint8_t Speed_Time=0;
    Position_Time++;
    Speed_Time++;
    if(Position_Time == 10)
    {
       Position_Time = 0;
       PID_Calculate(&Position_POS,0,Distance_data);
    }
    
     if(Speed_Time == 5)
     {
        Actual_Speed = Distance_data - Pre_Dis;
        Pre_Dis = Distance_data;
     }
//    if((Actual_Speed_last - Actual_Speed) > ERROR_MAX || (Actual_Speed_last - Actual_Speed) < -ERROR_MAX)
//        Actual_Speed = Actual_Speed_last;
//    else
//    {
//        Actual_Speed = Actual_Speed*0.3 + Actual_Speed_last*0.7;
//        Actual_Speed_last = Actual_Speed;
//    }
    
    PID_Calculate(&Speed_POS,Position_POS.PID_Out,Actual_Speed);

    Limit_Out(&Speed_POS.PID_Out,0,4500);   //扇叶仅正转   
}

