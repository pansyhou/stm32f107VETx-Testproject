/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;
CAN_FilterTypeDef CAN_FilterInitStructure;
CAN_RxHeaderTypeDef pRxMailbox;
/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */
  __HAL_RCC_CAN1_CLK_ENABLE();
  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */
  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_5TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  CAN_Filter_Config();
  /* USER CODE BEGIN CAN1_Init 2 */
  uint32_t *pTxMailbox;
  
  uint8_t temp[8]={0,0xcc};
  uint8_t can1Rx[8]={0};
  CAN_TxHeaderTypeDef CAN_TxHeaderInitStructure;
  CAN_TxHeaderInitStructure.StdId=0x200;
  CAN_TxHeaderInitStructure.IDE=CAN_ID_STD;
  CAN_TxHeaderInitStructure.RTR=CAN_RTR_DATA;
  CAN_TxHeaderInitStructure.DLC=8;
  HAL_CAN_Start(&hcan1);
  while (1)
  {
    HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeaderInitStructure,temp,pTxMailbox);

  }
  
  
  /* USER CODE END CAN1_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0|GPIO_PIN_1);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}
/* USER CODE BEGIN 1 */
void CAN_Filter_Config(void)
{
  
  CAN_FilterInitStructure.FilterBank=0; //指定筛选器组0,范围0-13
  CAN_FilterInitStructure.FilterMode=CAN_FILTERMODE_IDLIST;//筛选器模式为idlsit
  CAN_FilterInitStructure.FilterScale=CAN_FILTERSCALE_16BIT;//筛选器位宽为单个16bit
  CAN_FilterInitStructure.FilterIdLow=0x201;//低位iD
  CAN_FilterInitStructure.FilterFIFOAssignment=CAN_FILTER_FIFO0;
  /* 使能筛选器 */
  CAN_FilterInitStructure.FilterActivation=ENABLE;
  HAL_CAN_ConfigFilter(&hcan1,&CAN_FilterInitStructure);
  
  pRxMailbox.StdId=0x201;
  pRxMailbox.IDE=CAN_ID_STD;
  pRxMailbox.RTR=CAN_RTR_DATA;
  pRxMailbox.DLC=8;
  pRxMailbox.FilterMatchIndex=0;
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
  hcan1.Instance->sFIFOMailBox->RDTR

}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
