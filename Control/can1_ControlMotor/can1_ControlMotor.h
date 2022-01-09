#ifndef __CAN1_ControlMotor__H
#define __CAN1_ControlMotor__H

#include "main.h"
void Can1_ControlMotor_Exp_Mian();
void CAN_Filter_Config(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

#endif