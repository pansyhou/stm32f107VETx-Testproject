#ifndef __PID_H
#define __PID_H

#include "SYSConfig.h"

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
    fp32 Last_CurrentValues;
    fp32 CurrentValues;//当前值 

    fp32 H_Limited;//上限
    fp32 L_Limited;//上限

    int Circl_Cut;//这玩意是个啥？


    fp32 Output;
}PIDTypeDef;

fp32 limit(fp32 object,fp32 H_Limitation,fp32 L_Limitation);
fp32 PIDcal(PIDTypeDef *pid);
fp32 PID_Position_speed_Control(PIDTypeDef *SpeedPid,PIDTypeDef *PoisionPid,fp32 current_speed,fp32 current_poision,fp32 set_poision);
fp32 PID_STOP_Init(void);
void PID_Init(PIDTypeDef *PID_Init_t,fp32 kp,fp32 ki,fp32 kd,fp32 H_Limited,fp32 L_Limited);
#endif

