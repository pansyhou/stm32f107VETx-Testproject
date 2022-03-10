#ifndef __CAN1_RECEIVE_H
#define __CAN1_RECEIVE_H

#include "SYSConfig.h"


#define gimbal_yaw_to_chassis 2//旧版本can2 数据由can2传输给底盘板5684，底盘板再输出给yaw轴电机    1 -> can1发送   2 -> can2发送

//CAN接收ID
typedef enum
{
    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_M3508_MOTOR1_ID = 0x201,
    CAN_M3508_MOTOR2_ID = 0x202,
    CAN_M3508_MOTOR3_ID = 0x203,
    CAN_M3508_MOTOR4_ID = 0x204,

    CAN_TRIGGER_MOTORA_ID = 0x205,  //拨弹A
    CAN_TRIGGER_MOTORB_ID = 0x206,  //拨弹B
    CAN_PIT_MOTOR_ID = 0x207,       //pitch轴电机
    CAN_GIMBAL_ALL_ID = 0x1FF,      //
	

    /* 在云台部分代码的can2处调用此枚举。 */
		#if (gimbal_yaw_TO_chassis == 2)
	    CAN_YAW_MOTOR_ID = 0x208,

		#elif(gimbal_yaw_TO_chassis == 1)
		CAN_YAW_MOTOR_ID = 0x5ff,

		#endif

    //CAN_ENEMY_COLOR_ID = 0x3ff,
}CAN_msg_e;



//电机数据结构体
typedef struct
{
    uint16_t position;
    int16_t speed;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_position;


    int16_t angle;
    int16_t speed_filt;
    int16_t first_Flag;
    int32_t yaw_angle;
    int32_t pitch_angle;
    int32_t actual_Position;  //真实位置
}CAN_Motor_Measure_Data;


/**************** 函数声明 *****************/
void CAN1_Filter_Config(void);                                      //CAN1滤波器配置（不带CAN中断启动、CAN开启传输
void CAN1_Chassis_Receive(CAN_HandleTypeDef *hcan);                 //CAN1接收（大概是想收底盘四个电机
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);    //重定义CAN回调函数
const void CAN1_Get_ChassisMotor_Pointer(void);                     //底盘电机获取
#endif

