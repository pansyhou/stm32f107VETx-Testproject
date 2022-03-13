/************************** Dongguan-University of Technology -ACE**************************
 * @file gimbal_app.c
 * @author pansyhou侯文辉 (1677195845lyb@gmail.com)
 * @brief 云台初始化设置
 * @version 0.1
 * @date 2022-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 ************************** Dongguan-University of Technology -ACE***************************/
#include "gimbal_app.h"
#include "SYSConfig.h"

osThreadId GimbalTaskHandle;
static CAN_TxHeaderTypeDef Txmessage;   //发送CAN信息结构体

extern TIM_HandleTypeDef htim1;


//这里有段PWM输出，我也不知道是什么，暂时不写

void Gimbal_app_Init(void)
{
    //这里又来一段PWM，保守估计是灯？或者是os的时基

    osThreadDef(GimbalTask,Gimbal_Task,osPriorityRealtime,0,200);//栈为200
    GimbalTaskHandle = osThreadCreate(osThread(GimbalTask));
    
}


/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘板CAN1发送到云台Y电机和供弹电机
 *      
 * 
 * @param ESC_205 供弹电机1
 * @param ESC_206 2
 * @param ESC_207 p轴电机
 * @param ESC_208 y轴电机
 ************************** Dongguan-University of Technology -ACE***************************/
void Gimbal_app_CAN1_Fire(int16_t ESC_205,int16_t ESC_206, int16_t ESC_207, int16_t ESC_208 )
{
    uint32_t Tx_MailBox;    //发送邮箱
    uint8_t Data[8];        //发送数据数组

    CAN_TxHeaderTypeDef Txmessage;
    Txmessage.StdId = 0x1ff;       
    Txmessage.IDE = CAN_ID_STD;
    Txmessage.RTR = CAN_RTR_DATA;
    Txmessage.DLC = 8;

    Data[0] = (ESC_205 >> 8);   //高8位
    Data[1] = ESC_205;
	Data[2] = (ESC_206>>8);     //高8位
	Data[3] = ESC_206;
	Data[4] = (ESC_207>>8);
	Data[5] = ESC_207;
	Data[6] = (ESC_208>>8);
	Data[7] = ESC_208;

    HAL_CAN_AddTxMessage(&hcan1,&Txmessage,Data,&Tx_MailBox);
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief YAW轴接CAN2发送
 * 
 * @param ESC_208 
 ************************** Dongguan-University of Technology -ACE***************************/
void Gimbal_app_CAN2_YAW_SendMsg(int16_t ESC_208)
{
    uint32_t Tx_MailBox;    //发送邮箱
    uint8_t Data[8];        //发送数据数组

    CAN_TxHeaderTypeDef Txmessage;
    Txmessage.StdId = 0x1ff;       
    Txmessage.IDE = CAN_ID_STD;
    Txmessage.RTR = CAN_RTR_DATA;
    Txmessage.DLC = 8;

    Data[0] = (unsigned char)(0);
	Data[1] = (unsigned char)(0);
	Data[2] = (unsigned char)(0);
	Data[3] = (unsigned char)(0);
	Data[4] = (unsigned char)(0);
	Data[5] = (unsigned char)(0);
	Data[6] = (ESC_208>>8);
	Data[7] =  ESC_208;

    HAL_CAN_AddTxMessage(&hcan1,&Txmessage,Data,&Tx_MailBox);
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief yaw轴接CAN1，将云台计算好的输出发送到底盘板
 * 
 * @param ESC_208 
 ************************** Dongguan-University of Technology -ACE***************************/
void CAN2_Gimbal_YAW_SendMsg(int16_t ESC_208)
{
    uint32_t Tx_MailBox;    //发送邮箱
    uint8_t Data[8];        //发送数据数组

    CAN_TxHeaderTypeDef Txmessage;
    Txmessage.StdId = 0x500;       
    Txmessage.IDE = CAN_ID_STD;
    Txmessage.RTR = CAN_RTR_DATA;
    Txmessage.DLC = 8;

    Data[0] = (unsigned char)(0);
	Data[1] = (unsigned char)(0);
	Data[2] = (unsigned char)(0);
	Data[3] = (unsigned char)(0);
	Data[4] = (unsigned char)(0);
	Data[5] = (unsigned char)(0);
	Data[6] = (ESC_208>>8);
	Data[7] =  ESC_208;

    HAL_CAN_AddTxMessage(&hcan1,&Txmessage,Data,&Tx_MailBox);
}