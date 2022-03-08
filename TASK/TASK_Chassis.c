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
Chassis_Control_t chassis_control;



/**************** 函数声明 *****************/

void Chassis_Task(void const *argument);                          //底盘任务
void Chassis_Init(Chassis_Control_t *Chassis_Init_t);             //初始化函数
void Chassis_Control(Chassis_Control_t *Chassis_Ctrl_t);          //控制主函数
void Chassis_Motor_Data_Update(Chassis_Control_t *Chassis_MDU_t); // MDU=Motor_Data_Update
void Chassis_RC_MODE_Set(Chassis_Control_t *Chassis_RC_Mode_t);   //遥控器模式设置
void Chassis_Pid_Cal(Chassis_Control_t *Chassis_PID_t);           // PID计算
void Chassis_SentTo_Gimbal(Chassis_Control_t *Chassis_Gimbal_t);  //底盘发送数据至云台
void Chassis_Task_OFF(uint8_t options);                           //底盘全部关闭

/**************** 底盘任务 *****************/
void Chassis_Task(void const *argument)
{
    vTaskDelay(CHASSIS_TASK_INIT_TIME);

    Chassis_Init(&chassis_control);

    while (1)
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
    //获取遥控器控制结构体，通过指针传递方式传递信息
    Chassis_Init_t->RC_Chassis_Data = RC_Get_RC_Pointer();

    //获取底盘四个电机的指针数据
    CAN1_Get_ChassisMotor_Pointer();

    //检查遥控器数据有没有错误
    RC_Check_Data_IS_ERROR();

    //缺低通滤波

    //PID初始化
    Chassis_PID_Init(Chassis_Init_t);
    
    //底盘全部关闭
    Chassis_Task_OFF(0);

    //开机时初始化前进方向
    chassis_control.sign = GOFORWARD; //向前

    //更新数据
    Chassis_Motor_Data_Update(Chassis_Init_t);
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘控制函数
 *
 * @param Chassis_Ctrl_t 整体底盘数据结构体
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_Control(Chassis_Control_t *Chassis_Ctrl_t)
{
    RC_Check_Data_IS_ERROR(); //检查接收到的遥控器数据有没有误

    Chassis_Motor_Data_Update(Chassis_Ctrl_t); //更新motor电机测量到的数据

    Chassis_RC_MODE_Set(Chassis_Ctrl_t); //遥控器模式检查选择

    Chassis_Pid_Cal(Chassis_Ctrl_t); // PID计算
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 更新统一结构体的4个motor的测量的数据至底盘大结构体
 *
 * @param Chassis_MDU_t
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_Motor_Data_Update(Chassis_Control_t *Chassis_MDU_t) // MDU=Motor_Data_Update
{
    uint8_t i = 0;

    for (; i < 4; i++)
    {
        Chassis_MDU_t->motor[i].speed = Chassis_MDU_t->motor[i].Chassis_Motor_Measured_Data->speed;
        Chassis_MDU_t->motor[i].position = Chassis_MDU_t->motor[i].Chassis_Motor_Measured_Data->position;
        // Chassis_MDU_t->motor[i].accel=
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
 * @brief 底盘PID结构体初始化
 * 
 * @param Chassis_PID_Init_t 
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_PID_Init(Chassis_Control_t *Chassis_PID_Init_t)
{
    PID_Init(&Chassis_PID_Init_t->Chassis_Speed_PID[0],CHASSIS_SPEED_PID_kp,CHASSIS_LOCATION_PID_ki,CHASSIS_LOCATION_PID_kd,CHASSIS_SPEED_H_LIMITED,-CHASSIS_SPEED_L_LIMITED);
    PID_Init(&Chassis_PID_Init_t->Chassis_Speed_PID[1],CHASSIS_SPEED_PID_kp,CHASSIS_LOCATION_PID_ki,CHASSIS_LOCATION_PID_kd,CHASSIS_SPEED_H_LIMITED,-CHASSIS_SPEED_L_LIMITED);
    PID_Init(&Chassis_PID_Init_t->Chassis_Speed_PID[2],CHASSIS_SPEED_PID_kp,CHASSIS_LOCATION_PID_ki,CHASSIS_LOCATION_PID_kd,CHASSIS_SPEED_H_LIMITED,-CHASSIS_SPEED_L_LIMITED);
    PID_Init(&Chassis_PID_Init_t->Chassis_Speed_PID[3],CHASSIS_SPEED_PID_kp,CHASSIS_LOCATION_PID_ki,CHASSIS_LOCATION_PID_kd,CHASSIS_SPEED_H_LIMITED,-CHASSIS_SPEED_L_LIMITED);
)


/************************** Dongguan-University of Technology -ACE**************************
 * @brief PID计算
 *
 * @param Chassis_PID_t
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_Pid_Cal(Chassis_Control_t *Chassis_PID_t)
{
    Chassis_PID_t->motor[0].output = RM_Speed_PID(Chassis_PID_t->Chassis_Speed_PID, Chassis_PID_t->motor[0].speed_set, Chassis_PID_t->motor->speed, M3508_MAX_OUTPUT_CURRENT);
    Chassis_PID_t->motor[1].output = RM_Speed_PID(Chassis_PID_t->Chassis_Speed_PID, Chassis_PID_t->motor[1].speed_set, Chassis_PID_t->motor->speed, M3508_MAX_OUTPUT_CURRENT);
    Chassis_PID_t->motor[2].output = RM_Speed_PID(Chassis_PID_t->Chassis_Speed_PID, Chassis_PID_t->motor[2].speed_set, Chassis_PID_t->motor->speed, M3508_MAX_OUTPUT_CURRENT);
    Chassis_PID_t->motor[3].output = RM_Speed_PID(Chassis_PID_t->Chassis_Speed_PID, Chassis_PID_t->motor[3].speed_set, Chassis_PID_t->motor->speed, M3508_MAX_OUTPUT_CURRENT);
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘发送数据至云台
 *
 * @param Chassis_Gimbal_t
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_SentTo_Gimbal(Chassis_Control_t *Chassis_Gimbal_t)
{
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘全部关闭
 *
 * @param options 0:清楚底盘控制量 1：禁止底盘 2：禁止底盘加YAw轴和拨弹轮
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_Task_OFF(uint8_t options)
{
    //清除底盘控制量
    chassis_control.Speed_x_set = 0;
    chassis_control.Speed_y_set = 0;
    for (uint8_t i = 0; i < 4; i++)
    {
        chassis_control.motor[i].speed_set = 0;
        chassis_control.motor[i].output = 0;
    }
    chassis_control.Chassis_Mode = CHASSIS_STOP;
    if (options)
    {
        Chassis_app_CAN1_SendMsg(0, 0, 0, 0);
        if (options == 2)
        {
        }
    }
}