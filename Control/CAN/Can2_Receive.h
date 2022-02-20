#ifndef __CAN2_RECEIVE_H
#define __CAN2_RECEIVE_H

#include "SYSConfig.h"

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

#endif

