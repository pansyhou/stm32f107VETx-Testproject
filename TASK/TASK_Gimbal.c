#include "TASK_Gimbal.h"

//声明云台主控制变量
gimbal_control_t gimbal_control;

uint16_t rng1 = 0, rng2 = 0;

static uint16_t pitch_angle_init_flag = 0; // 标志位，初始化电机码盘用的
static uint16_t pitch_angle_count = 0;     // 计数值，初始化电机码盘用的

void GIMBAL_TASK(void const *argument)
{
    vTaskDelay(GIMBAL_TASK_INIT_TIME);
    //云台数据初始化
    Gimbal_Init(&gimbal_control);

    while (1)
    {
        if (pitch_angle_init_flag == 0)
        {
            Gimbal_Pitch_Init(&gimbal_control);
            pitch_angle_init_flag = 1;
        }
        //心跳任务
        LEDE2 = 0;
        //主任务
        Gimbal_Control_Work(&gimbal_control);

        //进行pitch轴3508电机和两个拨弹电机的控制，两个拨弹为-+，通过Can1发送

        Gimbal_app_CAN1_Fire(gimbal_control.Fire_task_control->GDA_output,
                             gimbal_control.Fire_task_control->GDB_output,
                             gimbal_control.pitch_c.output,
                             0); // y轴走Can1

        if (gimbal_yaw_to_chassis == 2)
            CAN2_Gimbal_YAW_SendMsg(gimbal_control.yaw_c.output); //直接发给电调
        //这里就不写版本1了，个人觉得会增大y轴延迟，发给底盘再发出去较麻烦

        vTaskDelay(GIMBAL_CONTROL_TIME_MS); //系统延迟
    }
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 云台数据初始化
 *
 * @param Gimbal_Init_t
 ************************** Dongguan-University of Technology -ACE***************************/
static void Gimbal_Init(gimbal_control_t *Gimbal_Init_t)
{
    /*********************获取相关指针********************/
    //遥控数据指针
    Gimbal_Init_t->gimbal_RC = RC_Get_RC_Pointer();
    // YAw轴指针
    Gimbal_Init_t->yaw_c.yaw_motor_measure = Get_YAW_Gimbal_Measure_Point();
    // Pitch轴指针
    Gimbal_Init_t->pitch_c.pitch_motor_measure = Get_Pitch_Measure_point();
    //自瞄指针
    Gimbal_Init_t->auto_c = Get_Auto_Control_Point();
    //火控指针
    Gimbal_Init_t->Fire_task_control = Get_Fire_Control_Point();

    /***************检查遥控器数值正确性********************/
    RC_Check_Data_IS_ERROR();

    /****************滤波初始化*********************/
    // pitch轴低通滤波
    first_order_filter_init(&(Gimbal_Init_t->pitch_c.LowFilt_Pitch_Data), Gimbal_Pitch_Fir_Ord_Low_Fil_Param);
    // p轴滑动滤波
    Sliding_Mean_Filter_Init(&Gimbal_Init_t->pitch_c.Slidmean_Pitch_Data);

#if (PITCH_PID_MODE == 1)

#elif (PITCH_PID_MODE == 2)
    // pitch轴PID初始化
    PID_Init(&Gimbal_Init_t->pitch_c.pitch_p_pid, GIMBAL_P_PITCH_P, GIMBAL_P_PITCH_I, GIMBAL_P_PITCH_D,
             PITCH_INIT_OUTPUT_LIMIT, -PITCH_INIT_OUTPUT_LIMIT);
    PID_Init(&Gimbal_Init_t->pitch_c.pitch_s_pid, GIMBAL_S_PITCH_P, GIMBAL_S_PITCH_I, GIMBAL_S_PITCH_D,
             PITCH_INIT_OUTPUT_LIMIT, -PITCH_INIT_OUTPUT_LIMIT);

    // Pitch自瞄
    PID_Init(&Gimbal_Init_t->pitch_c.pitch_auto_p_pid, GIMBAL_AUTO_INDUSTRY_P_PITCH_P, GIMBAL_AUTO_INDUSTRY_P_PITCH_I, GIMBAL_AUTO_INDUSTRY_P_PITCH_D,
             PITCH_INIT_OUTPUT_LIMIT, -PITCH_INIT_OUTPUT_LIMIT);
    PID_Init(&Gimbal_Init_t->pitch_c.pitch_auto_s_pid, GIMBAL_AUTO_INDUSTRY_S_PITCH_P, GIMBAL_AUTO_INDUSTRY_S_PITCH_I, GIMBAL_AUTO_INDUSTRY_S_PITCH_D,
             PITCH_INIT_OUTPUT_LIMIT, -PITCH_INIT_OUTPUT_LIMIT);

    /*--------------------设置开机状态--------------------*/
    gimbal_task_off();

#endif
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief Pitch轴码盘数据初始化
 *
 *
 * @param Gimbal_Pitch_Init_t
 ************************** Dongguan-University of Technology -ACE***************************/
static void Gimbal_Pitch_Init(gimbal_control_t *Gimbal_Pitch_Init_t)
{
#if (pitch_angle_position == 0)
    Gimbal_app_CAN1_Fire(0, 0, -4000, 0);
#endif

#if (pitch_angle_position == 1)
    pitch_angle_init_flag = 1;
#endif
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 云台主要控制函数
 *
 * @param Gimbal_CW_t
 ************************** Dongguan-University of Technology -ACE***************************/
static void Gimbal_Control_Work(gimbal_control_t *Gimbal_CW_t)
{
    //检查遥控器数据正确性
    RC_Check_Data_IS_ERROR();

    //通过串口三接收并保存MiniPc发来的数据
    MiniPC_Data_Deal();

    //发送敌人颜色，p轴角度、射速给视觉
    MiniPC_Send_Data(rng2, 25, (uint8_t)28);

    //云台模式选择
    Gimbal_remote_mode_choose(Gimbal_CW_t);
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 云台模式选择
 * @status already
 * @param Gimbal_Mode_Choose_t
 ************************** Dongguan-University of Technology -ACE***************************/
static void Gimbal_Remote_Mode_Choose(gimbal_control_t *Gimbal_Mode_Choose_t)
{
    gimbal_behaviour_mode_set(Gimbal_Mode_Choose_t);
}



/************************** Dongguan-University of Technology -ACE**************************
 * @brief 云台全部关闭
 * 
 * @status 未完成,缺自瞄控制量
 * 
 * @attention      0: 状态接切换
 *                 1: 初次进入状态
 *                 2: 清除初始化标志位
 ************************** Dongguan-University of Technology -ACE***************************/
void gimbal_task_off(void)
{
    gimbal_control.pitch_c.output = 0;
    gimbal_control.yaw_c.output = 0;
    gimbal_control.VisionStatus = Enemy_Disappear;

    // 自瞄控制量
    gimbal_control.auto_c->auto_pitch_angle=0.0f;
    gimbal_control.auto_c->auto_yaw_angle=0.0f;
    gimbal_control.auto_c->pitch_control_data=0.0f;
    gimbal_control.auto_c->yaw_control_data=0.0f;
    gimbal_control.pitch_c.Auto_record_location=0.0f;

    gimbal_control.gimbal_behaviour=GIMBAL_STOP;
    Gimbal_Stop(gimbal_control);
}


/************************** Dongguan-University of Technology -ACE**************************
 * @brief 云台正常控制
 * @status not ready
 * @test?  
 *  
 * @param Gimbal_Working_t 
 * @param gimbal_ch2 
 * @param gimbal_ch3 
 ************************** Dongguan-University of Technology -ACE***************************/
void Gimbal_Manual_Work(gimbal_control_t *Gimbal_Working_t, int16_t gimbal_ch2, int16_t gimbal_ch3)
{   
    float Gimbal_yaw_set_position = 1;        // yaw 轴位置设定

    /*************P轴滤波处理***************/
    //为什么全在对Output做处理？input捏？
    //均值滑窗滤波（有滞后）
    Gimbal_Working_t->pitch_c.output=Sliding_Mean_Filter(&Gimbal_Working_t->pitch_c.Slidmean_Pitch_Data,Gimbal_Working_t->pitch_c.output,55);
    //一阶低通滤波
    Gimbal_Working_t->pitch_c.output=first_order_filter(&Gimbal_Working_t->pitch_c.LowFilt_Pitch_Data,Gimbal_Working_t->pitch_c.output);

    /*************步进式PIP处理***************/
    Gimbal_Working_t->pitch_c.output=Motor_Position_Stepping(&Gimbal_Working_t->pitch_c.pitch_s_pid,
                                                            &Gimbal_Working_t->pitch_c.pitch_p_pid,
                                                            Gimbal_Working_t->pitch_c.pitch_motor_measure->pitch_angle,
                                                            Gimbal_Working_t->pitch_c.pitch_motor_measure->speed,
                                                            gimbal_ch3,
                                                            PITCH_OUTPUT_LIMIT
                                                            );
    
}