/************************** Dongguan-University of Technology -ACE**************************
 * @file TASK_Chassis.h
 * @author pansyhou侯文辉 (1677195845lyb@gmail.com)
 * @brief简介
 * @version 0.1
 * @date 2022-02-03
 *
 * @copyright Copyright (c) 2022
 *
 ************************** Dongguan-University of Technology -ACE************************** */

#ifndef __TASK_CHASSIS_H
#define __TASK_CHASSIS_H
#include "SYSConfig.h"

//os启动时间和控制任务周期
#define CHASSIS_TASK_INIT_TIME 5
#define CHASSIS_CONTROL_TIME   2


//底盘电机PID参数
#define CHASSIS_SPEED_PID_kp 8.0f
#define CHASSIS_SPEED_PID_ki 2.0f
#define CHASSIS_SPEED_PID_kd 0.0f

//底盘电机位置环参数

#define CHASSIS_LOCATION_PID_kp 100.0f
#define CHASSIS_LOCATION_PID_ki 2.0f
#define CHASSIS_LOCATION_PID_kd 0.0f

//底盘电机输出限制,vscode 里PID Init用不了，疯狂报错
#define CHASSIS_SPEED_H_LIMITED 5000.0f
#define CHASSIS_SPEED_L_LIMITED -5000.0f


//底盘模式
// typedef enum
// {
    
// }Chassis_Mode;


typedef struct
{
    const CAN_Motor_Measure_Data *Chassis_Motor_Measured_Data;

    fp32 accel; //加速度
    fp32 speed;
    fp32 speed_set;
    fp32 position;
    fp32 last_position;

    int16_t output;
} Chassis_MotorStructure;

//底盘整体数据结构体
typedef struct
{
    const RC_Ctl_t *RC_Chassis_Data;    //底盘遥控器数据指针
    CAN_Motor_Measure_Data *Yaw_Motor_Measurement;  //can2接收到的Yaw数据指针

    Chassis_MotorStructure motor[4];    //底盘电机数据
    PIDTypeDef Chassis_Speed_PID[4];    //底盘速度PID
    PIDTypeDef Chassis_location_PID[4]; //底盘位置环PID

    Chassis_mode_e Chassis_Mode;        //遥控器设置底盘状态的状态标志位

    fp32 Speed_x_set;//底盘设定速度 前进方向 前为正，单位 m/s
    fp32 Speed_y_set;//底盘设定速度 左右方向 左为正，单位 m/s

    fp32 Angel_Between_Chassis_Gimbal;//底盘与云台之间的角度?还是两个的角度

    first_order_filter_type_t LowFilt_chassis_vx;//低通滤波器结构体，vx
    first_order_filter_type_t LowFilt_chassis_vy;

    uint8_t sign;       //前后走标志
    uint8_t sign_last;  //延续前后走标志

}Chassis_Control_t;


/**************** 函数声明 *****************/

void Chassis_Task(void const * argument);                           //底盘任务
void Chassis_Init(Chassis_Control_t *Chassis_Init_t);               //初始化函数
void Chassis_Control(Chassis_Control_t *Chassis_Ctrl_t);            //控制主函数
void Chassis_Motor_Data_Update(Chassis_Control_t *Chassis_MDU_t);   //MDU=Motor_Data_Update
void Chassis_RC_MODE_Set(Chassis_Control_t *Chassis_RC_Mode_t);     //遥控器模式设置    
void Chassis_Pid_Cal(Chassis_Control_t *Chassis_PID_t);             //PID计算
void Chassis_SentTo_Gimbal(Chassis_Control_t *Chassis_Gimbal_t);    //底盘发送数据至云台
void Chassis_Task_OFF(uint8_t options);                             //底盘全部关闭
#define CHASSIS_AUTO_SPPED 0 //还没测试之前暂时不设置



/**********************运动加速度限制**********************/
#define STRAIGHT_ACCELERAD        3.5f      //直行底盘加速度限制
#define TRANSLATION_ACCELERAD     5.5f      //平移底盘加速度限制
#define CHASSIS_AUTO_SPPED				4000			 //21赛季为7000			
#define CHASSIS_BLOCKING_SPPED		    6700 //走位速度 ，挨打后的速度 7000  


#endif
