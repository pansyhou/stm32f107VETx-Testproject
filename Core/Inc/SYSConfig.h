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


/***************************** 各限制选择 *************************************/

#define pitch_angle_position  1         // p轴使用编码器    0 -> 不使用  1 -> 使用
#define chassis_using		  1         // 底盘运动         0 -> 不使用  1 -> 使用
#define hot_limit             0         // 热量限制         0 -> 不使用  1 -> 使用
#define yaw_angle_limit       0		    // yaw 轴角度限制   0 -> 不限制  1 -> 限制









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
#include "gimbal_app.h"
#include "automatic_strike.h"


/********************  freeRTOS part flies **********************/
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/********************  task part flies **********************/
#include "TASK_Chassis.h"
#include "TASK_Gimbal.h"
#include "TASK_Fire.h"


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

#include "fifo.h"
// 方向
#define	GOFORWARD	 1				//自动模式下的向前
#define	GOBACK		 0				//自动模式下的向后


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
#define RC_CAN1_TO_SEND_ID 0x400 //底盘发送遥控器数据到云台板的CAN STD ID
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;





//LED端口定义
#define LEDE0 PEout(0)  //遥控
#define LEDE1 PEout(1)  //底盘
#define LEDE2 PEout(2)	//云台	
#define LEDE3 PEout(3)  //火控	
#define LEDE4 PEout(4)	//安检
#define LEDB7 PBout(7)  //工作指示灯

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).M4同M3类似,只是寄存器地址变了.
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     


#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 

//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入


//系统状态结构体
typedef enum
{
	INF_STOP = 0, //停车模式
	INF_RC,       //遥控器模式
	INF_MK,       //键盘模式
} infantry_state_e;




/***************函数声明********************/
void system_state_set(infantry_state_e state);
infantry_state_e system_state_return(void);


#endif
