#ifndef __CHASSIS_APP_H
#define __CHASSIS_APP_H
#include "SYSConfig.h"
#include "Can1_Receive.h"
/******************函数声明*******************/
void Chassis_app_Init(void);    //底盘任务创建初始化
void Chassis_app_CAN1_SendMsg(int16_t ESC_201,int16_t ESC_202,int16_t ESC_203,int16_t ESC_204);//通过CAN1发送四个电机的信息
void Chassis_app_CAN2_SendMsg(int16_t ESC_201,int16_t ESC_202,int16_t ESC_203,int16_t ESC_204);//通过CAN2发送四个电机的信息
void Chassis_app_YAW_SendMsg(const CAN_Motor_Measure_Data *Can_YAW_Send);//底盘发送YAW数据到云台板
void Chassis_app_RC_CAN1_SendMsg(const RC_Ctl_t *CAN1_RC_Send_t);




#endif

