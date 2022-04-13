#include "imu.h"


const imu_data_t *get_imu_control_point(void);
void imu_control_init(void);

void imu_data_deal(void);
void imu_zero_correct(void);
void gyro_usart_iwdg(void);
void imu_data_reception(void);

/***************变量******************/
imu_data_t imu_data;
fifo_rx_def fifo_usart_rx_1;
fifo_rx_def *pfifo_imu = &fifo_usart_rx_1;

#define RC_RX_LEN 18
uint8_t RC_Rx[RC_RX_LEN];

#define USART1_IMU_RX_LEN 32
#define USART1_IMU_TX_LEN 32
uint8_t Usart1_IMU_Rx1[USART1_IMU_RX_LEN] = {0};
uint8_t Usart1_IMU_Rx2[USART1_IMU_RX_LEN] = {0};
uint8_t Usart1_IMU_Tx[USART1_IMU_TX_LEN] = {0};
uint8_t Usart1_IMU_Tx_Buffer[USART1_IMU_TX_LEN] = {0};

fifo_rx_def *pfifo_1 = &fifo_usart_rx_1;
fifo_rx_def fifo_usart_tx_1;


/************************** Dongguan-University of Technology -ACE**************************
 * @brief imu串口接收初始化
 * @status 没试的啊，我也不知道能不能用
 * 
 ************************** Dongguan-University of Technology -ACE***************************/
void imu_control_init(void)
{ 
  //发送配置
  { 
    //关闭DMA
    __HAL_DMA_DISABLE(&hdma_usart1_tx);

    hdma_usart1_rx.Instance->PAR=(uint32_t) & (USART1->DR); 
    hdma_usart1_tx.Instance->M0AR=(uint32_t)Usart1_IMU_Tx;
    hdma_usart1_tx.Instance->NDTR=USART1_IMU_TX_LEN;

    __HAL_DMA_ENABLE(&hdma_usart1_tx);
  }
  
  //接收，记得在cubemax里调整串口配置，与遥控器有所不同
  {
      //使用寄存器直接操作使能DMA串口接收
    SET_BIT(huart1.Instance->CR3,USART_CR3_DMAR);

    //使能空闲中断
    __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);

    //先失能DMA
    __HAL_DMA_DISABLE(&hdma_usart1_rx);

    //等待失能，也是直接操作寄存器
    while (hdma_usart1_rx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart1_rx);
    }
    
    hdma_usart1_rx.Instance->PAR=(uint32_t)&(USART1->DR);//设置DMA外设基地址
    hdma_usart1_rx.Instance->M0AR=(uint32_t)Usart1_IMU_Rx1;//设置缓冲区1
    hdma_usart1_rx.Instance->M1AR=(uint32_t)Usart1_IMU_Rx2;//设置缓冲器2
    hdma_usart1_rx.Instance->NDTR=USART1_IMU_RX_LEN;//设置数据长度

    //使能DMA 双缓存区
    SET_BIT(hdma_usart1_rx.Instance->CR,DMA_SxCR_DBM);

    //使能DMA接收
    __HAL_DMA_ENABLE(&hdma_usart1_rx);
  }
  //发送

}







/**
  * @brief      返回超陀螺仪结构体
  * @param[in]  none
  * @retval     imu_data_t *
  */
const imu_data_t *get_imu_control_point(void)
{
    return &imu_data;
}


/**
  * @brief      陀螺仪零飘校准
  * @param[in]  none
  * @retval     none
  * @attention  帧头  校验段0x12 0x34  帧尾
  *             通过放入数据到fifo缓冲区，
  *             然后在调用函数将缓冲区的数据放进DMA的数组里面，开启DMA传输
  */
void imu_zero_correct(void)
{
    uint8_t SendBuff_Correct[2];
    SendBuff_Correct[0] = 0x12;
    SendBuff_Correct[1] = 0x34;

    //usart1_dma_send(SendBuff_Correct, 2);

  HAL_UART_Transmit_DMA(&huart1,SendBuff_Correct,2);
}

/**
  * @brief      给陀螺仪喂狗粮
  * @param[in]  none
  * @retval     none
  * @attention  0x01
  *             通过放入数据到fifo缓冲区，
  *             然后在调用函数将缓冲区的数据放进DMA的数组里面，开启DMA传输
  */
void gyro_usart_iwdg(void)
{
    uint8_t gyro[1];
    gyro[0] = 0x01;
	
	// usart1_dma_send(gyro, 1);
  HAL_UART_Transmit_DMA(&huart1,gyro,2);
}

/**
  * @brief      接收陀螺仪回传数值
  * @param[in]  none
  * @retval     imu_data结构体
  * @attention  0xFF   0xFE
  *             通过接收fifo队列的数据接收（具体看相对应的串口文件）
  */
//int BMI160_Yaw_Angle;  //DEBUG用
//int BMI160_Pitch_Angle;
//int BMI160_Gyro_X;
void imu_data_reception(void)
{
	uint8_t imu_buff_read[32];

	uint32_t length_imu = fifo_read_buff(pfifo_imu, imu_buff_read, 32);
    if (length_imu)
    {
		imu_data.length = length_imu;
		for (uint16_t i = 0; i < length_imu; i++)
		{
			if (imu_buff_read[i] == 0xfe && imu_buff_read[i + 13] == 0xee)
			{
				imu_data.Gyro_X = ((short)(imu_buff_read[i + 1] << 8 | imu_buff_read[i + 2])) / 100.0f;
				imu_data.Gyro_Y = ((short)(imu_buff_read[i + 3] << 8 | imu_buff_read[i + 4])) / 100.0f;
				imu_data.Gyro_Z = -((short)(imu_buff_read[i + 5] << 8 | imu_buff_read[i + 6])) / 100.0f; //陀螺仪模块朝上，加负号，反之
				imu_data.roll_angle = ((short)(imu_buff_read[i + 7] << 8 | imu_buff_read[i + 8])) / 100.0f;
				imu_data.pitch_angle = ((short)(imu_buff_read[i + 9] << 8 | imu_buff_read[i + 10])) / 100.0f;
				imu_data.yaw_angle = -((short)(imu_buff_read[i + 11] << 8 | imu_buff_read[i + 12])) / 100.0f; //陀螺仪模块朝上，加负号，反之
				
//				BMI160_Yaw_Angle = imu_data.yaw_angle;
//				BMI160_Pitch_Angle = imu_data.pitch_angle;
//				BMI160_Gyro_X = imu_data.Gyro_X;
				
				i = i + 13;
			}
		}		
	}
	else
	{
		//没有数据
	}
	if (pfifo_imu->error)
    {
		// 接收错误
        pfifo_imu->error = 0; 
    }
}


void IMU_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    if(huart1.Instance->SR & UART_FLAG_RXNE )
    {
        __HAL_UART_CLEAR_PEFLAG(&huart1);//先对PE位 置0，不然读不到数据（？）
    }
    else if (huart1.Instance->SR & UART_FLAG_IDLE)//进入空闲中断后开始转运数据？
    {
        __HAL_UART_CLEAR_PEFLAG(&huart1);
        static uint16_t this_time_rx_len=0;//当前接收数据的长度
        /* 缓冲区0 */
        if((hdma_usart1_rx.Instance->CR & DMA_SxCR_CT) == RESET)//获取双缓冲模式下的Current target
        {
            //关闭DMA
            __HAL_DMA_DISABLE(&hdma_usart1_rx);

            //当前接收数据的长度=设定长度-剩余长度36-18=18
            this_time_rx_len=Sbus_RX_Buffer_Num-hdma_usart1_rx.Instance->NDTR;

            //重新设置数据长度
            hdma_usart1_rx.Instance->NDTR=Sbus_RX_Buffer_Num;

            //设置缓冲区1
            hdma_usart1_rx.Instance->CR |= DMA_SxCR_CT;

            //开启DMA
            __HAL_DMA_ENABLE(&hdma_usart1_rx);
            if(this_time_rx_len==RC_FRAME_LENGTH)//处理数据
            {
                RC_DataProcess(Sbus_RX_Buffer[0],&rc_ctl);
            }
        }
        else//缓冲区1部分
        {
            //关闭DMA
            __HAL_DMA_DISABLE(&hdma_usart1_rx);

            //当前接收数据的长度=设定长度-剩余长度36-18=18
            this_time_rx_len=Sbus_RX_Buffer_Num-hdma_usart1_rx.Instance->NDTR;

            //重新设置数据长度
            hdma_usart1_rx.Instance->NDTR=Sbus_RX_Buffer_Num;

            //设置缓冲区0
            hdma_usart1_rx.Instance->CR &= ~(DMA_SxCR_CT);
            //这段稍微和昭廷的不一样，
            //DMA2_Stream2 ->CR &= ~(DMA_SxCR_CT);//待研究

            //开启DMA
            __HAL_DMA_ENABLE(&hdma_usart1_rx);
            if(this_time_rx_len==RC_FRAME_LENGTH)//处理数据
            {
                RC_DataProcess(Sbus_RX_Buffer[1],&rc_ctl);
            }
        }
    }
}
