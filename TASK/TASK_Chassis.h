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

//功率限制
#define POWER_LIMIT 0

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

//键盘按键结构体
typedef struct
{
	float key_w;
    float key_s;
    float key_a;
    float key_d;
	
	float last_key_w;
    float last_key_s;
    float last_key_a;
    float last_key_d;
} key_wsad_t;


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
    const gimbal_yaw_receive_t *gimbal_re_data; //云台板处理数据，数据由can2传输给底盘板，底盘板再输出给yaw轴电机
	
    uint8_t SuperCap_discharge_flag;  //超级电容放电标志位

    Chassis_MotorStructure motor[4];    //底盘电机数据
    PIDTypeDef Chassis_Speed_PID[4];    //底盘速度PID
    PIDTypeDef Chassis_location_PID[4]; //底盘位置环PID
    PIDTypeDef chassis_rotate_pid;   //旋转pid
	PIDTypeDef chassis_power_pid;   //功率限制pid

    Chassis_mode_e Chassis_Mode;        //遥控器设置底盘状态的状态标志位

    fp32 Speed_x_set;//底盘设定速度 前进方向 前为正，单位 m/s
    fp32 Speed_y_set;//底盘设定速度 左右方向 左为正，单位 m/s
    fp32 speed_z_set; //底盘设定旋转角速度，逆时针为正 单位 rad/s

    fp32 Angel_Between_Chassis_Gimbal;//底盘与云台之间的角度?还是两个的角度

    first_order_filter_type_t LowFilt_chassis_vx;//低通滤波器结构体，vx
    first_order_filter_type_t LowFilt_chassis_vy;

    uint8_t sign;       //前后走标志
    uint8_t sign_last;  //延续前后走标志

    fp32 chassis_speed_gain;
	fp32 chassis_last_speed_gain;

    fire_task_t *fire_c;
	key_wsad_t key;
}Chassis_Control_t;


/**************** 函数声明 *****************/

void Chassis_Task(void const * argument);                           //底盘任务
void Chassis_Init(Chassis_Control_t *Chassis_Init_t);               //初始化函数
void Chassis_Control(Chassis_Control_t *Chassis_Ctrl_t);            //控制主函数
void Chassis_Motor_Data_Update(Chassis_Control_t *Chassis_MDU_t);   //MDU=Motor_Data_Update
void Chassis_RC_MODE_Set(Chassis_Control_t *Chassis_RC_Mode_t);     //遥控器模式设置    
void Chassis_Pid_Cal(Chassis_Control_t *Chassis_PID_t);             //PID计算
void Chassis_SentTo_Gimbal(Chassis_Control_t *Chassis_Gimbal_t);    //底盘发送数据至云台
void Chassis_Task_OFF(uint8_t options);     
void Chassis_set_remote(Chassis_Control_t *chassis_set_t,int16_t ch0,int16_t ch1,int16_t ch2);                        //底盘全部关闭
#define CHASSIS_AUTO_SPPED 0 //还没测试之前暂时不设置


//这个是哨兵的
// /**********************运动加速度限制**********************/
// #define STRAIGHT_ACCELERAD        3.5f      //直行底盘加速度限制
// #define TRANSLATION_ACCELERAD     5.5f      //平移底盘加速度限制
// #define CHASSIS_AUTO_SPPED				4000			 //21赛季为7000			
// #define CHASSIS_BLOCKING_SPPED		    6700 //走位速度 ，挨打后的速度 7000 

//底盘测试模式 宏定义 0 为不使用测试模式
#define CHASSIS_TEST_MODE   0

/**********************底盘电机pid参数**********************/
#define CHASSIS_MOTOR1_PID_Kp    9.0f
#define CHASSIS_MOTOR1_PID_Ki    0.0f
#define CHASSIS_MOTOR1_PID_Kd    0.5f

#define CHASSIS_MOTOR2_PID_Kp    9.0f //8
#define CHASSIS_MOTOR2_PID_Ki    0.0f
#define CHASSIS_MOTOR2_PID_Kd    0.5f

#define CHASSIS_MOTOR3_PID_Kp    9.0f
#define CHASSIS_MOTOR3_PID_Ki    0.0f
#define CHASSIS_MOTOR3_PID_Kd    0.5f

#define CHASSIS_MOTOR4_PID_Kp    9.0f //8
#define CHASSIS_MOTOR4_PID_Ki    0.0f
#define CHASSIS_MOTOR4_PID_Kd    0.5f

#define CHASSIS_LOCATION_PID_P  100.0f  //位置,ctrl
#define CHASSIS_LOCATION_PID_I  0.0f
#define CHASSIS_LOCATION_PID_D  10.8f

#define CHASSIS_ROTATE_FOLLOW_P  7.0f  //PID   8.0  13
#define CHASSIS_ROTATE_FOLLOW_I  0.01f   //0.01
#define CHASSIS_ROTATE_FOLLOW_D  20.0f   //5.02   10.02   2

/**********************低通滤波比例**********************/
#define CHASSIS_FIRST_ORDER_FILTER_K  0.0410f  //0.0110f 越小越平稳，灵敏度越低；越高输出不稳，但灵敏度更高  |  0.26f  |  0.0097f  |  0.0510f
#define CHASSIS_MK_SECOND_FILTERING   (0.8f)



//TODO:要理解这几个数值的意思
/**********************运动加速度限制**********************/
#define STRAIGHT_ACCELERAD        3.5f      //直行底盘加速度限制
#define TRANSLATION_ACCELERAD     5.5f      //平移底盘加速度限制
#define ROTATING_ACCELERAD        19.0f     //旋转底盘加速度限制


#define CHASSIS_ROTATION_SPEED_1    (600)
#define CHASSIS_ROTATION_SPEED_2    (750)
#define CHASSIS_ROTATION_SPEED_3    (1200)  //900

#define CHASSIS_ROTATION_SPEED        100      //小陀螺的旋转速度  2000
#define CHASSIS_ROTATION_MOVE_SPEED  (800*0.8f)   //小陀螺移动时为防止轨迹失真减转速   1700
#define CHASSIS_TWIST_SPEED           600      //扭腰速度  1600

#define NORMAL_FORWARD_BACK_SPEED 	660.0f   //键盘普通直行速度
#define NORMAL_LEFT_RIGHT_SPEED   	660.0f   //键盘普通平移速度

#define HIGH_FORWARD_BACK_SPEED 	750.0f     //键盘加速直行速度  600.0f 
#define HIGH_LEFT_RIGHT_SPEED   	750.0f     //键盘加速平移速度



#endif
