#ifndef __CAN1_ControlMotor__H
#define __CAN1_ControlMotor__H

#include "main.h"
typedef struct
{
    fp32 kp;//������
    fp32 ki;//������
    fp32 kd;//΢����

    fp32 P_Out;
    fp32 I_out;
    fp32 D_Out;

    fp32 error[2];//��һ������һ�εĲ�ֵ���ڶ��������µĲ�ֵ

    fp32 SetValues;//�趨ֵ
    fp32 Last_CurrentValues;
    fp32 CurrentValues;//��ǰֵ 

    fp32 H_Limited;//����
    fp32 L_Limited;//����

    int Circl_Cut;

    fp32 Output;
}PIDTypeDef;
void Can1_ControlMotor_Exp_Mian(void);
void CAN_Filter_Config(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

#endif

