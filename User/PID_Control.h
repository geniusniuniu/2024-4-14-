#ifndef __PID_Control_H
#define __PID_Control_H


extern float Actual_Speed;
extern float Pre_Dis;
typedef struct
{
	float Err;			  //����ƫ��ֵ
	float Err_last;		  //������һ��ƫ��ֵ

	float Kp,Ki,Kd;		  //������������֡�΢��
	float Integral;       // ������
	float Integral_Last;  // ��һ�λ���
	float PID_Out;		  //�����������
	
	float Out_Limit;      // ����޷�
	float Integral_Limit;  // �����޷� 
}PID_InitTypeDef;


extern PID_InitTypeDef Speed_POS;
extern PID_InitTypeDef Position_POS;
extern PID_InitTypeDef Power_POS;
void PID_Init(PID_InitTypeDef *PID_Struct, float Kp, float Ki, float Kd, float Out_limit, float Integral_limit);		//PID��ʼ��
void PID_Calculate(PID_InitTypeDef *PID_Struct, float Exp_Val, float Act_Val);		//PID����



//***********************************************************����ʽPID**************************************************//
typedef struct{
    float Kp;
    float Ki;
    float Kd;
	
    float error;
    float last_error;
    float last_last_error;
    float last_out;
	
    float out;
    float outmax;
    float outmin;
	
    uint8_t use_lowpass_filter;
    float lowpass_filter_factor;
	
} PID_Incremental;

extern PID_Incremental Position_INC;
extern PID_Incremental Power_INC;

void PID_Incremental_Init(PID_Incremental *pid, float Kp, float Ki, float Kd,float Out_Limit, uint8_t use_lowpass_filter);
float PID_Incremental_Calc(PID_Incremental *pid, float setpoint, float input_value);



////�޷�����
void Limit_Out(float *Output,float Limit_Min,float Limit_Max);

float Num2Abs(float x);
void PID_Control(void);

#endif
