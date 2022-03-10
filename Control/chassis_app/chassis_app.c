/************************** Dongguan-University of Technology -ACE**************************
 * @file chassis_app.c
 * @author pansyhou侯文辉 (1677195845lyb@gmail.com)
 * @brief 底盘初始化
 * @version 0.1
 * @date 2022-02-19
 * 
 * @copyright Copyright (c) 2022
 * 
 ************************** Dongguan-University of Technology -ACE***************************/


#include "chassis_app.h"
#include "SYSConfig.h"

osThreadId ChassisTaskHandle;
static CAN_TxHeaderTypeDef Txmessage;   //发送CAN信息结构体



/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘任务创建初始化
 * 
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_app_Init(void)
{
    //创建底盘任务
    osThreadDef(ChassisTask,Chassis_Task,osPriorityRealtime,0,200);
    ChassisTaskHandle = osThreadCreate(osThread(ChassisTask),NULL);

}
/************************** Dongguan-University of Technology -ACE**************************
 * @brief 通过CAN1发送四个电机的信息
 *        ESC：Electronic Speed Contro 电调
 * @param ESC_201 201电调
 * @param ESC_202 类推
 * @param ESC_203 
 * @param ESC_204 
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_app_CAN1_SendMsg(int16_t ESC_201,int16_t ESC_202,int16_t ESC_203,int16_t ESC_204)
{
    uint32_t Tx_MailBox;    //发送邮箱
    uint8_t Data[8];        //发送数据数组

    CAN_TxHeaderTypeDef Txmessage;
    Txmessage.StdId = 0x200;       //这id可以用宏再配置，等我回学校记得写
    Txmessage.IDE = CAN_ID_STD;
    Txmessage.RTR = CAN_RTR_DATA;
    Txmessage.DLC = 8;

    Data[0] = (ESC_201 >> 8);   //高8位
    Data[1] = ESC_201;
	Data[2] = (ESC_202>>8);     //高8位
	Data[3] = ESC_202;
	Data[4] = (ESC_203>>8);
	Data[5] = ESC_203;
	Data[6] = (ESC_204>>8);
	Data[7] = ESC_204;

    HAL_CAN_AddTxMessage(&hcan1,&Txmessage,Data,&Tx_MailBox);
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 通过CAN2发送四个电机的信息，调试时有重名风险，但问题不大
 *        ESC：Electronic Speed Contro 电调
 * @param ESC_201 
 * @param ESC_202 
 * @param ESC_203 
 * @param ESC_204 
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_app_CAN2_SendMsg(int16_t ESC_201,int16_t ESC_202,int16_t ESC_203,int16_t ESC_204)
{
    uint32_t Tx_MailBox;    //发送邮箱
    uint8_t Data[8];        //发送数据数组

    CAN_TxHeaderTypeDef Txmessage;
    Txmessage.StdId = 0x200;       //这id可以用宏再配置，等我回学校记得写
    Txmessage.IDE = CAN_ID_STD;
    Txmessage.RTR = CAN_RTR_DATA;
    Txmessage.DLC = 8;

    Data[0] = (ESC_201 >> 8);   //高8位
    Data[1] = ESC_201;
	Data[2] = (ESC_202>>8);     //高8位
	Data[3] = ESC_202;
	Data[4] = (ESC_203>>8);
	Data[5] = ESC_203;
	Data[6] = (ESC_204>>8);
	Data[7] = ESC_204;

    HAL_CAN_AddTxMessage(&hcan2,&Txmessage,Data,&Tx_MailBox);
}



/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘发送YAW数据到云台板
 * 
 * @param Can_YAW_Send 
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_app_YAW_SendMsg(const CAN_Motor_Measure_Data *Can_YAW_Send)
{
    uint32_t Tx_MailBox;    //发送邮箱
    uint8_t Data[8];        //发送数据数组

    CAN_TxHeaderTypeDef Txmessage;
    Txmessage.StdId = YAW_CAN_TO_SEND_ID;       //这id可以用宏再配置，等我回学校记得写
    Txmessage.IDE = CAN_ID_STD;
    Txmessage.RTR = CAN_RTR_DATA;
    Txmessage.DLC = 8;

    Data[0] = (unsigned char)(Can_YAW_Send->position>>8);       //高8位
    Data[1] = (unsigned char)(Can_YAW_Send->position);

	Data[2] = (unsigned char)(Can_YAW_Send->speed>>8);          //高8位
	Data[3] = (unsigned char)(Can_YAW_Send->speed);

	Data[4] = 0;
	Data[5] = 0;
	Data[6] = 0;
	Data[7] = 0;

    HAL_CAN_AddTxMessage(&hcan2,&Txmessage,Data,&Tx_MailBox);
}





/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘CAN1遥控器数据发送（到底发送什么过去，我都不知道
 * 
 * @param CAN1_RC_Send_t 
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_app_RC_CAN1_SendMsg(const RC_Ctl_t *CAN1_RC_Send_t)
{
    uint32_t Tx_MailBox;    //发送邮箱
    uint8_t Data[8];        //发送数据数组

    CAN_TxHeaderTypeDef Txmessage;
    Txmessage.StdId = RC_CAN1_TO_SEND_ID;  
    Txmessage.IDE = CAN_ID_STD;
    Txmessage.RTR = CAN_RTR_DATA;
    Txmessage.DLC = 8;

    //有空记得改遥控器发送的数据，我都不知道发什么过去
    Data[0]=(unsigned char)(CAN1_RC_Send_t->rc.ch1>>8);
    Data[1]=(unsigned char)(CAN1_RC_Send_t->rc.ch1);
    Data[2]=(unsigned char)(CAN1_RC_Send_t->rc.ch2>>8);
    Data[3]=(unsigned char)(CAN1_RC_Send_t->rc.ch2);
    Data[4]=(unsigned char)(CAN1_RC_Send_t->rc.ch3>>8);
    Data[5]=(unsigned char)(CAN1_RC_Send_t->rc.ch3);
    Data[6]=(unsigned char)(CAN1_RC_Send_t->rc.s1);
    Data[7]=(unsigned char)(CAN1_RC_Send_t->rc.s2);


    HAL_CAN_AddTxMessage(&hcan2,&Txmessage,Data,&Tx_MailBox);
}