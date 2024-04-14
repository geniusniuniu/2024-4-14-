#ifndef __Timer_H
#define __Timer_H

extern uint32_t Timmer_NumCount4;
extern uint32_t Timmer_NumCount3;
extern uint32_t Timmer_NumCount2;
extern uint32_t Task_Delay;

void TIM3_Int_Init(int32_t ARR,int32_t PSC);


#endif
