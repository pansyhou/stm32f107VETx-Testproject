/************************** Dongguan-University of Technology -ACE**************************
 * @file SYSConfig.h
 * @author pansyhou侯文辉 (1677195845lyb@gmail.com)
 * @brief简介 一个包含全部定义的头文件
 * @version 0.1
 * @date 2022-01-29
 *
 * @copyright Copyright (c) 2022
 *
 ************************** Dongguan-University of Technology -ACE************************** */
#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H

#define NEED_STRING 0 //是否需要string.h

/************************** main head files **************************/
#include "main.h"
#include "stdlib.h"
#include "stdio.h"
#if NEED_STRING
#include "string.h"
#endif

/************************ control part files *************************/
#ifndef __REMOTECONTROL_H
#include "RemoteControl.h"
#endif

#ifndef __CAN1_RECEIVE_H
#include "Can1_Receive.h"
#endif

#ifndef __CAN2_RECEIVE_H
#include "Can2_Receive.h"
#endif

#ifndef __CHASSIS_BEHAVIOUR_H
#include "chassis_behaviour.h"
#endif

#include "chassis_app.h"

/********************  freeRTOS part flies **********************/
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/********************  task part flies **********************/
#include "TASK_Chassis.h"


/************************** Core part files ***************************/
#include "usart.h"
#include "can.h"
#include "dma.h"
#include "gpio.h"
#include "cmsis_os.h"
/************************** Algorithm part files ***************************/
#ifndef __PID_H
#include "pid.h"
#endif

#ifndef __RMMOTOR_H
#include "rmmotor.h"
#endif

#ifndef __MATHS_H
#include "maths.h"
#endif

/* 底盘电机移动速度设定 */
#define M3508_MAX_OUTPUT_CURRENT 5000 // m3508电机最大电流输出
#define M2006_MAX_OUTPUT_CURRENT 9500 // m2006电机最大电流输出

#define MAX_MOTOR_CAN_INPUT 2000.0f   // 3508最大电流输入
#define MAX_MOTOR_CAN_OUTPUT 16000.0f // 3508最大电流输出


/************电机 传动比*减速比 ***************/
#define YAW_RATIO (5 * 19)     // Yaw轴
#define PITCH_RATIO (1.5 * 19) // Pitch轴
#define CHASSIS_RATIO (1 * 19) //底盘电机减速比
#define Sec_YAW_RATIO (3 * 1)  //副Yaw轴
#define PITCH_GR (1.5)         // pitch传动比

#define YAW_CAN_TO_SEND_ID 0x5ff //底盘发送YAW数据至云台板的的CAN STD ID

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

#endif
