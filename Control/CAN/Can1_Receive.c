#include "Can1_Receive.h"

//申明底盘四个电机变量
static CAN_Motor_Measure_Data motor_Chassis[4];

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 文件包含以下内容：
 *      1.CAN1滤波器配置
 *      2.CAN1接收
 *      3.重定义的CAN回调函数
 *  
 ************************** Dongguan-University of Technology -ACE***************************/

void CAN1_Filter_Config(void);                                      //CAN1滤波器配置（不带CAN中断启动、CAN开启传输
void CAN1_Chassis_Receive(CAN_HandleTypeDef *hcan);                 //CAN1接收（大概是想收底盘四个电机
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);    //重定义CAN回调函数

extern Chassis_Control_t chassis_control;


const void CAN1_Get_ChassisMotor_Pointer(void)
{
    //直接霸王硬上弓赋值
    uint8_t i=0;
    for ( i = 0; i < 4; i++)
    {
        chassis_control.motor[i].Chassis_Motor_Measured_Data=&motor_Chassis[i];
    }
}



/************************** Dongguan-University of Technology -ACE**************************
 * @brief CAN1滤波器配置（不带CAN中断启动、CAN开启传输
 *
 ************************** Dongguan-University of Technology -ACE***************************/
void CAN1_Filter_Config(void)
{
    CAN_FilterTypeDef CAN1_FilterInitStructure;
    CAN1_FilterInitStructure.FilterBank = 0;                      //指定筛选器组0,范围0-13
    CAN1_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;  //筛选器模式为idlsit
    CAN1_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT; //筛选器位宽为32bit
    CAN1_FilterInitStructure.FilterMaskIdHigh = 0x0000;
    CAN1_FilterInitStructure.FilterMaskIdLow = 0x0000;
    CAN1_FilterInitStructure.SlaveStartFilterBank = 14;
    CAN1_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;

    /* 使能筛选器 */
    CAN1_FilterInitStructure.FilterActivation = ENABLE;
    HAL_CAN_ConfigFilter(&hcan1, &CAN1_FilterInitStructure);
    if (HAL_CAN_ConfigFilter(&hcan1, &CAN1_FilterInitStructure) != HAL_OK)
    {
        Error_Handler();
    }
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief CAN1接收（大概是想收底盘四个电机
 * 
 * @param hcan 
 ************************** Dongguan-University of Technology -ACE***************************/
void CAN1_Chassis_Receive(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef pRxMailbox; //接收信息邮箱结构体
    uint8_t Rx_Data[8];             //接收缓存数组

    if (HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO0, &pRxMailbox, Rx_Data) == HAL_OK) //接收数据后分类
    {
        switch (pRxMailbox.StdId)
        {   
            /*************底盘四个电机接收*************/
            case CAN_M3508_MOTOR1_ID:
            {
                motor_Chassis[0].position=      (uint16_t)((Rx_Data[0]<<8) + (Rx_Data[1]));
                motor_Chassis[0].speed=         (uint16_t)((Rx_Data[2]<<8) + (Rx_Data[3]));
                motor_Chassis[0].given_current= (uint16_t)((Rx_Data[4]<<8) + (Rx_Data[5]));
                motor_Chassis[0].temperate=     (uint16_t)((Rx_Data[6]<<8) + (Rx_Data[7]));
            }break;
            
            case CAN_M3508_MOTOR2_ID:
            {
                motor_Chassis[1].position=      (uint16_t)((Rx_Data[0]<<8) + (Rx_Data[1]));
                motor_Chassis[1].speed=         (uint16_t)((Rx_Data[2]<<8) + (Rx_Data[3]));
                motor_Chassis[1].given_current= (uint16_t)((Rx_Data[4]<<8) + (Rx_Data[5]));
                motor_Chassis[1].temperate=     (uint16_t)((Rx_Data[6]<<8) + (Rx_Data[7]));
            }break;

            case CAN_M3508_MOTOR3_ID:
            {
                motor_Chassis[2].position=      (uint16_t)((Rx_Data[0]<<8) + (Rx_Data[1]));
                motor_Chassis[2].speed=         (uint16_t)((Rx_Data[2]<<8) + (Rx_Data[3]));
                motor_Chassis[2].given_current= (uint16_t)((Rx_Data[4]<<8) + (Rx_Data[5]));
                motor_Chassis[2].temperate=     (uint16_t)((Rx_Data[6]<<8) + (Rx_Data[7]));
            }break;

            case CAN_M3508_MOTOR4_ID:
            {
                motor_Chassis[3].position=      (uint16_t)((Rx_Data[0]<<8) + (Rx_Data[1]));
                motor_Chassis[3].speed=         (uint16_t)((Rx_Data[2]<<8) + (Rx_Data[3]));
                motor_Chassis[3].given_current= (uint16_t)((Rx_Data[4]<<8) + (Rx_Data[5]));
                motor_Chassis[3].temperate=     (uint16_t)((Rx_Data[6]<<8) + (Rx_Data[7]));
            }break;

            default:
                break;
        }
    }
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 重定义CAN回调函数
 *
 * @param hcan can句柄
 ************************** Dongguan-University of Technology -ACE***************************/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == &hcan1)
    {
        CAN1_Chassis_Receive(hcan);
    }
    if (hcan == &hcan2)
    {
        CAN2_Chassis_Receive(hcan);
    }
}