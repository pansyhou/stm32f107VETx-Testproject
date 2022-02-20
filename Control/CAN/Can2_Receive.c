#include "Can2_Receive.h"


//声明Roll轴和Yaw轴电机变量
static CAN_Motor_Measure_Data Motor_Roll,Motor_Yaw;

/************************** Dongguan-University of Technology -ACE**************************
 * @brief CAN2滤波器配置（不带CAN中断启动、CAN开启传输
 * 
 ************************** Dongguan-University of Technology -ACE***************************/
void CAN2_Filter_Config(void)
{
    CAN_FilterTypeDef CAN2_FilterInitStructure;
    CAN2_FilterInitStructure.FilterBank = 0;                      //指定筛选器组0,范围0-13
    CAN2_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;  //筛选器模式为idlsit
    CAN2_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT; //筛选器位宽为32bit
    CAN2_FilterInitStructure.FilterMaskIdHigh = 0x0000;
    CAN2_FilterInitStructure.FilterMaskIdLow = 0x0000;
    CAN2_FilterInitStructure.SlaveStartFilterBank = 14;
    CAN2_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;

    /* 使能筛选器 */
    CAN2_FilterInitStructure.FilterActivation = ENABLE;
    HAL_CAN_ConfigFilter(&hcan2, &CAN2_FilterInitStructure);
    if (HAL_CAN_ConfigFilter(&hcan2, &CAN2_FilterInitStructure) != HAL_OK)
    {
        Error_Handler();
    }
}

void CAN2_Chassis_Receive(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef pRxMailbox; //接收信息邮箱结构体
    uint8_t Rx_Data[8];             //接收缓存数组
    if (HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO0, &pRxMailbox, Rx_Data) == HAL_OK) //接收数据后分类
    {
        switch (pRxMailbox.StdId)
        {
            case 
        }
    }
}