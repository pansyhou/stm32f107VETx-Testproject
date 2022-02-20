/************************** Dongguan-University of Technology -ACE**************************
 * @file TASK_Chassis.c
 * @author pansyhou侯文辉 (1677195845lyb@gmail.com)
 * @brief 底盘控制任务
 *        命名格式: 
 *              Chassis：Task_Chassis
 *              RC：RemoteControl
 * @version 0.1
 * @date 2022-02-11
 ************************** Dongguan-University of Technology -ACE***************************/

#include "TASK_Chassis.h"
#include "SYSConfig.h"
Chassis_Control_t chassis_control;

/**************** 函数声明 *****************/

void Chassis_Task(void const * argument);                           //底盘任务
void Chassis_Init(Chassis_Control_t *Chassis_Init_t);               //初始化函数
void Chassis_Control(Chassis_Control_t *Chassis_Ctrl_t);            //控制主函数
void Chassis_Motor_Data_Update(Chassis_Control_t *Chassis_MDU_t);   //MDU=Motor_Data_Update
void Chassis_RC_MODE_Set(Chassis_Control_t *Chassis_RC_Mode_t);     //遥控器模式设置    
void Chassis_Pid_Cal(Chassis_Control_t *Chassis_PID_t);             //PID计算
void Chassis_SentTo_Gimbal(Chassis_Control_t *Chassis_Gimbal_t);    //底盘发送数据至云台


/**************** 底盘任务 *****************/
void Chassis_Task(void const * argument)
{
    vTaskDelay(CHASSIS_TASK_INIT_TIME);

    Chassis_Init(&chassis_control);

    while(1)
    {
        //底盘控制主函数
        Chassis_Control(&chassis_control);
    }

}



/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘初始化函数
 * 
 * @param Chassis_Init_t 整体底盘数据结构体
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_Init(Chassis_Control_t *Chassis_Init_t)
{

}


/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘控制函数
 * 
 * @param Chassis_Ctrl_t 整体底盘数据结构体
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_Control(Chassis_Control_t *Chassis_Ctrl_t)
{
    RC_Check_Data_IS_ERROR();                   //检查接收到的遥控器数据有没有误

    Chassis_Motor_Data_Update(Chassis_Ctrl_t);  //更新motor电机测量到的数据

    Chassis_RC_MODE_Set(Chassis_Ctrl_t);        //遥控器模式检查选择

    Chassis_Pid_Cal(Chassis_Ctrl_t);            //PID计算




}


/************************** Dongguan-University of Technology -ACE**************************
 * @brief 更新统一结构体的4个motor的测量的数据至上一级
 * 
 * @param Chassis_MDU_t 
 ************************** Dongguan-University of Technology -ACE***************************/

void Chassis_Motor_Data_Update(Chassis_Control_t *Chassis_MDU_t)    //MDU=Motor_Data_Update
{
    uint8_t i=0;

    for (;i<4;i++)
    {
        Chassis_MDU_t->motor[i].speed=Chassis_MDU_t->motor[i].Chassis_Motor_Measured_Data->speed;
        Chassis_MDU_t->motor[i].position=Chassis_MDU_t->motor[i].Chassis_Motor_Measured_Data->position;
        //Chassis_MDU_t->motor[i].accel=

    }

}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 遥控器模式设置
 * 
 * @param Chassis_RC_Mode_t 
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_RC_MODE_Set(Chassis_Control_t *Chassis_RC_Mode_t)
{
    Chassis_Behaviour_Mode_Set(Chassis_RC_Mode_t);
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief PID计算
 * 
 * @param Chassis_PID_t 
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_Pid_Cal(Chassis_Control_t *Chassis_PID_t)
{
    Chassis_PID_t->motor[0].output=RM_Speed_PID(Chassis_PID_t->Chassis_Speed_PID,Chassis_PID_t->motor[0].speed_set,Chassis_PID_t->motor->speed,M3508_MAX_OUTPUT_CURRENT);
    Chassis_PID_t->motor[1].output=RM_Speed_PID(Chassis_PID_t->Chassis_Speed_PID,Chassis_PID_t->motor[1].speed_set,Chassis_PID_t->motor->speed,M3508_MAX_OUTPUT_CURRENT);
    Chassis_PID_t->motor[2].output=RM_Speed_PID(Chassis_PID_t->Chassis_Speed_PID,Chassis_PID_t->motor[2].speed_set,Chassis_PID_t->motor->speed,M3508_MAX_OUTPUT_CURRENT);
    Chassis_PID_t->motor[3].output=RM_Speed_PID(Chassis_PID_t->Chassis_Speed_PID,Chassis_PID_t->motor[3].speed_set,Chassis_PID_t->motor->speed,M3508_MAX_OUTPUT_CURRENT);
}


/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘发送数据至云台
 * 
 * @param Chassis_Gimbal_t 
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_SentTo_Gimbal(Chassis_Control_t *Chassis_Gimbal_t)
{
    
}

