#ifndef __MATHS_H
#define __MATHS_H

#include "SYSConfig.h"

//运动加速度限制斜坡结构体
typedef __packed struct
{
    float Input;      //当前取样值
    float Last_Input; //上次取样值
    float Output;     //输出值
    float acc_now;    //当前加速度
    float acc_limit;  //需要限制的加速度
}Acceleration_Control_type_t;


//滑动均值滤波参数（浮点）
typedef __packed struct
{
    float Input;     //当前取样值
    int count_num;   //取样次数
    float Output;    //滤波输出
    float Sum;       //累计总和
    float FIFO[250]; //队列
    int sum_flag;    //已经够250个标志
} sliding_mean_filter_type_t;

//一阶低通滤波参数
typedef __packed struct
{
	fp32 input;        //输入数据
	fp32 last_input;   //上次数据
	fp32 out;          //滤波输出的数据
	fp32 num;          //滤波参数
}first_order_filter_type_t;


#endif