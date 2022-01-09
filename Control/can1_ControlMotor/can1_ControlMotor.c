//#include "can1_ControlMotor.h"
//#include "stm32f4xx_hal_rcc.h"
//#include "stm32f4xx_hal.h"
//#include "main.h"
//#include "stm32f4xx_hal_can.h"
//void Can1_ControlMotor_Exp_Mian()
//{
//    /* 使能相关时钟 */
//    __HAL_RCC_CAN1_CLK_ENABLE();
//    __HAL_RCC_GPIOD_CLK_ENABLE();

//    /* 定义can1的句柄 */
//    CAN_HandleTypeDef hcan1;

//    /* 定义GPIO初始化结构体 */
//    GPIO_InitTypeDef GPIO_InitStruct;

//    /* 定义CAN1发送处理结构体 */
//    CAN_TxHeaderTypeDef CAN_TxHeaderInitStructure;

//    /* 初始化GPIO口 */
//    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
//    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

//    /* 配置can工作模式 */
//    hcan1.Instance = CAN1;
//    hcan1.Init.Prescaler = 3;
//    hcan1.Init.Mode = CAN_MODE_NORMAL;
//    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
//    hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
//    hcan1.Init.TimeSeg2 = CAN_BS2_5TQ;
//    hcan1.Init.TimeTriggeredMode = DISABLE;
//    hcan1.Init.AutoBusOff = DISABLE;
//    hcan1.Init.AutoWakeUp = DISABLE;
//    hcan1.Init.AutoRetransmission = DISABLE;
//    hcan1.Init.ReceiveFifoLocked = DISABLE;
//    hcan1.Init.TransmitFifoPriority = DISABLE;
//    if (HAL_CAN_Init(&hcan1) != HAL_OK)
//    {
//        Error_Handler();
//    }

//    uint32_t *pTxMailbox;
//    uint8_t temp[8] = {3, 51, 3, 51, 1, 1, 1, 1};
//    CAN_TxHeaderTypeDef CAN_TxHeaderInitStructure1;
//    CAN_TxHeaderInitStructure1.StdId = 0x200;
//    CAN_TxHeaderInitStructure1.IDE = CAN_ID_STD;
//    CAN_TxHeaderInitStructure1.RTR = CAN_RTR_DATA;
//    CAN_TxHeaderInitStructure1.DLC = 8;
//    //启动传输
//    HAL_CAN_Start(&hcan1);
//    while (1)
//    {
//        HAL_CAN_AddTxMessage(&hcan1, &CAN_TxHeaderInitStructure1, temp, pTxMailbox);
//    }
//}