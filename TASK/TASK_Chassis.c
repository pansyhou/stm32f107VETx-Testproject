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
void Chassis_Accelerated_Control(int16_t *ch0, int16_t *ch1, int16_t *ch2);//底盘加速度限制斜坡函数

/**************** 底盘任务 *****************/
void Chassis_Task(void const *argument)
{
    vTaskDelay(CHASSIS_TASK_INIT_TIME);

    Chassis_Init(&chassis_control);

    while (1)
    {
        //底盘控制主函数
        Chassis_Control(&chassis_control);


        taskENTER_CRITICAL(); //进入临界区
        //底盘发送数据到云台
        Chassis_SentTo_Gimbal(&chassis_control);


        taskEXIT_CRITICAL();     
        
        vTaskDelay(CHASSIS_CONTROL_TIME);    
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

    // PID初始化
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
 *  @status 计算底盘和云台的差角,未完成
 * @param Chassis_Ctrl_t 整体底盘数据结构体
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_Control(Chassis_Control_t *Chassis_Ctrl_t)
{
    RC_Check_Data_IS_ERROR(); //检查接收到的遥控器数据有没有误

    //计算底盘和云台的差角,未完成
    Chassis_Ctrl_t->Angel_Between_Chassis_Gimbal=Chassis_Ctrl_t->gimbal_re_data->chassis_gimbal_angel;
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
    PID_Init(&Chassis_PID_Init_t->Chassis_Speed_PID[0], CHASSIS_SPEED_PID_kp, CHASSIS_LOCATION_PID_ki, CHASSIS_LOCATION_PID_kd, CHASSIS_SPEED_H_LIMITED, -CHASSIS_SPEED_L_LIMITED);
    PID_Init(&Chassis_PID_Init_t->Chassis_Speed_PID[1], CHASSIS_SPEED_PID_kp, CHASSIS_LOCATION_PID_ki, CHASSIS_LOCATION_PID_kd, CHASSIS_SPEED_H_LIMITED, -CHASSIS_SPEED_L_LIMITED);
    PID_Init(&Chassis_PID_Init_t->Chassis_Speed_PID[2], CHASSIS_SPEED_PID_kp, CHASSIS_LOCATION_PID_ki, CHASSIS_LOCATION_PID_kd, CHASSIS_SPEED_H_LIMITED, -CHASSIS_SPEED_L_LIMITED);
    PID_Init(&Chassis_PID_Init_t->Chassis_Speed_PID[3], CHASSIS_SPEED_PID_kp, CHASSIS_LOCATION_PID_ki, CHASSIS_LOCATION_PID_kd, CHASSIS_SPEED_H_LIMITED, -CHASSIS_SPEED_L_LIMITED);
}

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

static int send_sign = 0;

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘发送数据至云台,
 *
 * @param Chassis_Gimbal_t
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_SentTo_Gimbal(Chassis_Control_t *Chassis_Gimbal_t)
{
    if ((Chassis_Gimbal_t->Chassis_Mode != CHASSIS_STANDBY) || (Chassis_Gimbal_t->Chassis_Mode != CHASSIS_STOP))
    {
        if (send_sign == 0)
        {
            Chassis_app_RC_CAN1_SendMsg(Chassis_Gimbal_t->RC_Chassis_Data); //否则为遥控模式
            send_sign = 1;
        }
        if (send_sign == 1)
        {
#if (gimbal_yaw_to_chassis == 1)
            Chassis_app_YAW_SendMsg(Chassis_Gimbal_t->Yaw_Motor_Measurement);
#endif
            send_sign = 0;
        }
    }
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

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘加速度限制 斜坡函数实现
 *         斜坡函数可以让输入信号变的更加平缓，减少系统超调，从而优化系统的时间相应，
 * https://my.oschina.net/u/4303162/blog/3365594
 * 
 * @param ch0 
 * @param ch1 
 * @param ch2 
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_Accelerated_Control(int16_t *ch0, int16_t *ch1, int16_t *ch2)
{
    static int16_t last_ch[3] = {0, 0, 0};
    int16_t temp[3];

    temp[0] = *ch0 - last_ch[0];
    temp[1] = *ch1 - last_ch[1];
    temp[2] = *ch1 - last_ch[2];
    //下面是哨兵的
    // if (chassis_control.Chassis_Mode == CHASSIS_AUTO) //底盘为自动模式
    // {
    //     if (float_abs(temp[0]) > TRANSLATION_ACCELERAD)
    //         *ch0 = last_ch[0] + temp[0] / float_abs(temp[0]) * TRANSLATION_ACCELERAD;
    //     if (float_abs(temp[1]) > STRAIGHT_ACCELERAD)
    //         *ch1 = last_ch[1] + temp[1] / float_abs(temp[1]) * STRAIGHT_ACCELERAD;
    // }
    // if (chassis_control.Chassis_Mode == CHASSIS_REMOTECONTROL)//遥控模式
    // {
    //     if (float_abs(temp[0]) > TRANSLATION_ACCELERAD)
    //         *ch0 = last_ch[0] + temp[0] / float_abs(temp[0]) * TRANSLATION_ACCELERAD;

    //     if (float_abs(temp[1]) > STRAIGHT_ACCELERAD)
    //         *ch1 = last_ch[1] + temp[1] / float_abs(temp[1]) * STRAIGHT_ACCELERAD;
    // }

    if(chassis_control.RC_Chassis_Data->rc.s2==RC_SW_UP)//遥控模式
    {
        if (float_abs(temp[0]) > TRANSLATION_ACCELERAD)
            *ch0 = last_ch[0] + temp[0] / float_abs(temp[0]) * TRANSLATION_ACCELERAD;
        if (float_abs(temp[1]) > STRAIGHT_ACCELERAD)
            *ch1 = last_ch[1] + temp[1] / float_abs(temp[1]) * STRAIGHT_ACCELERAD;

        if (chassis_control.Chassis_Mode == CHASSIS_TWIST_WAIST || chassis_control.Chassis_Mode == CHASSIS_ROTATION) //扭腰模式下才用旋转加速度限制
        {
            if (float_abs(temp[2]) > ROTATING_ACCELERAD)
                *ch2 = last_ch[2] + temp[2] / float_abs(temp[2]) * ROTATING_ACCELERAD;
        }
    }
    if(chassis_control.RC_Chassis_Data->rc.s2==RC_SW_MID)//键盘模式
    {
        if (float_abs(temp[0]) > TRANSLATION_ACCELERAD)
            *ch0 = last_ch[0] + temp[0] / float_abs(temp[0]) * TRANSLATION_ACCELERAD;
        if (float_abs(temp[1]) > STRAIGHT_ACCELERAD)
            *ch1 = last_ch[1] + temp[1] / float_abs(temp[1]) * STRAIGHT_ACCELERAD;

        if (chassis_control.Chassis_Mode == CHASSIS_TWIST_WAIST) //扭腰模式下才用旋转加速度限制
        {
            if (float_abs(temp[2]) > ROTATING_ACCELERAD)
                *ch2 = last_ch[2] + temp[2] / float_abs(temp[2]) * ROTATING_ACCELERAD;
        }
    }

    last_ch[0]=*ch0;
    last_ch[1]=*ch1;
    last_ch[2]=*ch2;

    
}

void Chassis_set_remote(Chassis_Control_t *chassis_set_t,int16_t ch0,int16_t ch1,int16_t ch2)
{
#ifdef POWER_LIMIT //功率限制
    chassis_power_limit_control(chassis_set_f);//TODO:缺功率限制函数
#endif

    //先对输入量进行斜坡函数处理
    Chassis_Accelerated_Control(&ch0,&ch1,&ch2);

    //一阶低通滤波计算
    first_order_filter(&(chassis_set_t->LowFilt_chassis_vx),-ch0);
    first_order_filter(&(chassis_set_t->LowFilt_chassis_vy),ch1);

    //底盘遥控器模式
    if(chassis_set_t->Chassis_Mode==CHASSIS_REMOTECONTROL)//遥控器模式
    {
        chassis_set_t->Speed_x_set=5.0f*ch0;
        chassis_set_t->Speed_y_set=5.0f*ch1;
        

    }
}


