#ifndef __CAN1_ControlMotor__H
#define __CAN1_ControlMotor__H

#include "main.h"
typedef struct
{
    fp32 kp;//比例项
    fp32 ki;//积分项
    fp32 kd;//微分项

    fp32 P_Out;
    fp32 I_out;
    fp32 D_Out;

    fp32 error[2];//第一个是上一次的差值，第二个是最新的差值

    fp32 SetValues;//设定值
    fp32 CurrentValues;//当前值 

    fp32 H_Limited;//上限
    fp32 L_Limited;//上限



    fp32 Output;
}PIDTypeDef;
void Can1_ControlMotor_Exp_Mian(void);
void CAN_Filter_Config(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

#endif