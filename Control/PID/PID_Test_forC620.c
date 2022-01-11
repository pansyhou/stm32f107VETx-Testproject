#include "main.h"
#include "PID_Test_forC620.h"

/**
 * @brief 一个PI控制器
 *
 * @param motor
 * @param set_Speed
 * @param current_Speed
 * @return fp32
 */


fp32 PIDcal(PIDTypeDef *pid)
{
    //差值，0为旧的，1是新的
    pid->error[1] = pid->SetValues - pid->CurrentValues;

    pid->P_Out = (pid->kp * pid->error[1]);
    pid->I_out += (pid->ki * pid->error[1]);
    pid->D_Out = (pid->kd*(pid->error[1]-pid->error[0]));

    //积分微分限幅
    pid->I_out=limit(pid->I_out,pid->H_Limited,pid->L_Limited);
    pid->D_Out=limit(pid->D_Out,pid->H_Limited,pid->L_Limited);

    pid->Output=pid->P_Out+pid->I_out+pid->D_Out;
    //输出限幅
    pid->Output=limit(pid->Output,pid->H_Limited,pid->L_Limited);

    pid->error[0]=pid->error[1];
    
    return pid->Output;
}

fp32 limit(fp32 object, fp32 H_Limitation, fp32 L_Limitation)
{
    if (object > H_Limitation)
        object = H_Limitation;
    else if (object < L_Limitation)
        object = L_Limitation;
    return object;
}
