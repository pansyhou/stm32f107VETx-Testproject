#ifndef __AUTO_STRIKE__H
#define __AUTO_STRIKE__H
#include "SYSConfig.h"

/************视觉相关结构体**********/
typedef struct
{
    float auto_pitch_angle;      //视觉传回P轴差角
    float auto_yaw_angle;        //视觉传回Y轴差角
    float last_Auto_Pitch_Angle; //上一次的P轴差角
    float last_Auto_Yaw_Angle;   //上一次的Y轴差角
    float len;                   //视觉传回距离
    float auto_yaw_speed;        //视觉回传数据算出的角速度
    float auto_pitch_sum;        //传回P轴角度积分
    float auto_pitch_angle_kf;   //电控卡尔曼处理后的P轴差角
    float auto_yaw_angle_kf;     //电控卡尔曼处理后的Y轴差角

    int16_t auto_lost_data_count;  //丢失目标计数
    int16_t auto_lost_data_flag;   //丢失目标标志位

    float pitch_control_data;     //P轴视觉控制量
    float yaw_control_data;       //y轴视觉控制量

} Vision_Auto_Data_t;

/**********************函数声明*****************/


Vision_Auto_Data_t *Get_Auto_Control_Point(void);
void MiniPC_Data_Deal(void);
#endif // !__AUTO_STRIKE__H
