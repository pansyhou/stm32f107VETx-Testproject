#ifndef __IMU_H__
#define __IMU_H__
#include "SYSConfig.h"

typedef struct
{
	uint16_t length;
	
    float Accel_X;  //转换成实际的X轴加速度，
    float Accel_Y;  //转换成实际的Y轴加速度，
    float Accel_Z;  //转换成实际的Z轴加速度，
	
    float Temp;     //转换成实际的温度，单位为摄氏度
	
    float Gyro_X;   //转换成实际的X轴角加速度，
    float Gyro_Y;   //转换成实际的Y轴角加速度，
    float Gyro_Z;   //转换成实际的Z轴角加速度
	
    float yaw_angle;
    float pitch_angle;
    float roll_angle;
}imu_data_t;

/***********函数声明****************/
const imu_data_t *get_imu_control_point(void);
void imu_control_init(void);

void imu_data_deal(void);
void imu_zero_correct(void);
void gyro_usart_iwdg(void);
void imu_data_reception(void);




#endif // !__IMU_H__
