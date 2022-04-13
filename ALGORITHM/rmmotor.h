#ifndef __RMMOTOR_H
#define __RMMOTOR_H

#include "SYSConfig.h"




/**************函数声明**************/

int16_t RM_Speed_PID(PIDTypeDef *SpeedPID_t, int16_t Set_speed, int16_t Actual_Speed, int16_t limit_values);//简简单单速度环PID控制
int16_t RM_Speed_Position_PID(PIDTypeDef *SpeedPID_t, PIDTypeDef *PositionPID_t, int16_t Actual_Speed, int16_t Actual_Position, int16_t SetPosition, int16_t limit_values);//位置速度闭环控制
void    RM_Motor_Actual_Poisition(CAN_Motor_Measure_Data *rmMotor, int16_t gear_Ratio, int16_t lap_encoder);//计算真实的码盘值
int16_t RM_Angle_Limiting_Int16(int16_t Angle_Err, int16_t lap_encoder);//临角处理16位（对应角度正值）
int32_t RM_Check_CodeValue(int32_t value, int16_t gear_Ratio, int16_t lap_encoder);//过临界值复位码盘值 （限制于360度的码盘值循环 DJI电机）
int16_t RM_Check_Motor_Block(int16_t position);//堵转检测
float   RM_Get_Yaw_AngleDifference(const CAN_Motor_Measure_Data *Yaw_Motor, int16_t Ratio);//Yaw轴电机与底盘中心差角获取
int16_t RM_Encoder_Real(int32_t read_code);//多圈绝对值编码器数据转换
int16_t RM_Yaw_Actual_Code_Conversion(int16_t actual_code, int16_t max_code, int16_t middle_code);//Yaw轴真实位置（多圈编码器）（左正右负）
int16_t Motor_Position_Stepping(PIDTypeDef *S_pid, PIDTypeDef *P_pid, int16_t actual_Position, int16_t actual_speed, int16_t set_Position, int16_t current_limit);


#endif
