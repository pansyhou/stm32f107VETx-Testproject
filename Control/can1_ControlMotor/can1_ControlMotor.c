#include "can1_ControlMotor.h"
//引用can1的句柄
extern fp32 PIDcal(PIDTypeDef *pid);
extern CAN_HandleTypeDef hcan1;
extern fp32 PID_STOP_Init(void);
extern fp32 PID_Position_speed_Control(PIDTypeDef *SpeedPid,PIDTypeDef *PoisionPid,fp32 current_speed,fp32 current_poision,fp32 set_poision);
uint8_t can1Tx[8] = {0};
uint8_t can1Rx[8] = {0};
CAN_FilterTypeDef CAN_FilterInitStructure;
CAN_RxHeaderTypeDef pRxMailbox;
PIDTypeDef C620Control;
PIDTypeDef C620Speed;
PIDTypeDef C620Poisition;
motor_Rx motor1;
int16_t output;
void Can1_ControlMotor_Exp_Mian(void)
{
  //相关时钟使能、初始化GPIO口hal自动给我们生成了
  HAL_CAN_MspInit(&hcan1);
  // HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);

  /* 配置发送结构体 */
  uint32_t *pTxMailbox;
  CAN_TxHeaderTypeDef CAN_TxHeaderInitStructure1;
  CAN_TxHeaderInitStructure1.StdId = 0x200;
  CAN_TxHeaderInitStructure1.IDE = CAN_ID_STD;
  CAN_TxHeaderInitStructure1.RTR = CAN_RTR_DATA;
  CAN_TxHeaderInitStructure1.DLC = 8;
//  C620Control.kp = 0.01;
//  C620Control.ki = 0;
//  C620Control.kd = 3;
//  C620Control.H_Limited = 5000;
//  C620Control.L_Limited = -1000;
//  C620Control.SetValues = 2500;
  C620Speed.H_Limited = 1500;
  C620Speed.L_Limited = -1500;  
	C620Speed.kp = 2.5;
  C620Speed.ki = 0.1;
  C620Speed.kd = 1.3;
  C620Poisition.H_Limited = 2000;
  C620Poisition.L_Limited = -2000;
  C620Poisition.kp = 1;
  C620Poisition.ki = 0.08;
  C620Poisition.kd = 1.1;
	PID_STOP_Init();                                                   //刹车结构体初始化
  CAN_Filter_Config();                                               //筛选器初始化
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); //启动接收
  HAL_CAN_Start(&hcan1);                                             //启动传输
  C620Poisition.SetValues = 4096; //角度设定值
  while (1)
  {
    output=PID_Position_speed_Control(&C620Speed,&C620Poisition,motor1.speed,motor1.angle,4096);
//		C620Control.CurrentValues=motor1.speed;//测试用
//		output=PIDcal(&C620Control);
//		C620Poisition.CurrentValues=motor1.angle;
//		output=PIDcal(&C620Poisition);
    can1Tx[0] = output >> 8;
    can1Tx[1] = output;
    HAL_CAN_AddTxMessage(&hcan1, &CAN_TxHeaderInitStructure1, can1Tx, pTxMailbox);
    HAL_Delay(10);
  }
}

/* 筛选器初始化 */
void CAN_Filter_Config(void)
{
  CAN_FilterInitStructure.FilterBank = 0;                      //指定筛选器组0,范围0-13
  CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;  //筛选器模式为idlsit
  CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT; //筛选器位宽为单个16bit
  CAN_FilterInitStructure.FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.SlaveStartFilterBank = 14;
  CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;

  /* 使能筛选器 */
  CAN_FilterInitStructure.FilterActivation = ENABLE;
  HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterInitStructure);
  if (HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterInitStructure) != HAL_OK)
  {
    Error_Handler();
  }
  /* 配置接收信息邮箱（？的句柄 */
  pRxMailbox.StdId = 0x201;
  pRxMailbox.IDE = CAN_ID_STD;
  pRxMailbox.RTR = CAN_RTR_DATA;
  pRxMailbox.DLC = 8;
}

/* 重定义接收中断回调函数 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if (hcan->Instance == CAN1)
  {
    HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO0, &pRxMailbox, can1Rx); //获取数据
    /* 处理数据 */
    motor1.angle = (uint16_t)((can1Rx[0] << 8 | can1Rx[1]));
    motor1.speed = (uint16_t)((can1Rx[2] << 8 | can1Rx[3]));
    motor1.anper = (uint16_t)((can1Rx[4] << 8 | can1Rx[5]));
    motor1.temperature = (uint16_t)((can1Rx[6]));
  }
  /* USER CODE END 1 */
}