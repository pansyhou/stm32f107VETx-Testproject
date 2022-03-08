#include "pid.h"

/**
 * @brief 一个PI控制器
 *
 * @param motor
 * @param set_Speed
 * @param current_Speed
 * @return fp32
 */
PIDTypeDef PID_STOP;

/**
 * @brief 刹车结构体初始化
 * 
 * @return fp32 
 */
fp32 PID_STOP_Init(void)
{
    PID_STOP.kp = 3;
    PID_STOP.ki = 0;
    PID_STOP.kd = 3;
    PID_STOP.H_Limited = 2000;
    PID_STOP.L_Limited = -2000;
    PID_STOP.SetValues = 0;
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief PID初始化
 * 
 * @param PID_Init_t 
 * @param kp 
 * @param ki 
 * @param kd 
 * @param H_Limited 输出上限
 * @param L_Limited 输出下限
 ************************** Dongguan-University of Technology -ACE***************************/
void PID_Init(PIDTypeDef *PID_Init_t,fp32 kp,fp32 ki,fp32 kd,float H_Limited,fp32 L_Limited)
{
    PID_Init_t->kp=kp;
    PID_Init_t->ki=ki;
    PID_Init_t->kd=kd;
    PID_Init_t->H_Limited=H_Limited;
    PID_Init_t->L_Limited=L_Limited;
}


fp32 PIDcal(PIDTypeDef *pid)
{
    //差值，0为旧的，1是新的
    pid->error[1] = pid->SetValues - pid->CurrentValues;

    pid->P_Out = (pid->kp * pid->error[1]);
    pid->I_out += (pid->ki * pid->error[1]);
    pid->D_Out = (pid->kd * (pid->error[1] - pid->error[0]));

    //积分微分限幅
    pid->I_out = limit(pid->I_out, pid->H_Limited, pid->L_Limited);
    pid->D_Out = limit(pid->D_Out, pid->H_Limited, pid->L_Limited);

    pid->Output = pid->P_Out + pid->I_out + pid->D_Out;
    //输出限幅
    pid->Output = limit(pid->Output, pid->H_Limited, pid->L_Limited);

    pid->error[0] = pid->error[1];
    pid->Last_CurrentValues = pid->CurrentValues;

    return pid->Output;
}



/**
 * @brief 位置速度双环pid，包含了防跑飞功能？
 * 
 * @param SpeedPid 速度环PID结构体指针
 * @param PoisionPid 位置环（外环）PID指针
 * @param current_speed 当前速度
 * @param current_poision 当前位置
 * @param set_poision 设置位置
 * @return fp32 电流值
 */
fp32 PID_Position_speed_Control(PIDTypeDef *SpeedPid, PIDTypeDef *PoisionPid, fp32 current_speed, fp32 current_poision, fp32 set_poision)
{
    if (PoisionPid->Circl_Cut < 2)//大概是跑两圈？
    {
        PoisionPid->SetValues = set_poision;

        PoisionPid->CurrentValues = current_poision;

        SpeedPid->CurrentValues = current_speed;
				if (PoisionPid->Last_CurrentValues - PoisionPid->CurrentValues > 3000 || PoisionPid->Last_CurrentValues - PoisionPid->CurrentValues < -3000)
            PoisionPid->Circl_Cut++;
        SpeedPid->SetValues = PIDcal(PoisionPid);
        fp32 output = PIDcal(SpeedPid);
        return output;
    }
    else if (PoisionPid->Circl_Cut >= 2&&PoisionPid->Circl_Cut<10)//防跑飞刹车时间在这调
    {	
				PID_STOP.CurrentValues=current_speed;
				PoisionPid->Circl_Cut++;
        return PIDcal(&PID_STOP);
    }
		else
		{
				PID_STOP.CurrentValues=current_speed;
				PoisionPid->Circl_Cut = 0;
        return PIDcal(&PID_STOP);
		}
}


fp32 limit(fp32 object, fp32 H_Limitation, fp32 L_Limitation)
{
    if (object > H_Limitation)
        object = H_Limitation;
    else if (object < L_Limitation)
        object = L_Limitation;
    return object;
}

