/************************** Dongguan-University of Technology -ACE**************************
 * @file RemoteControl.c
 * @author pansyhou侯文辉 (1677195845lyb@gmail.com)
 * @brief简介
 * 遥控器相关文件
 * 虽然说现在什么都不会，不敢写，但是还是要写一点出来
 * @version 0.1
 * @date 2022-01-26
 *
 * @copyright Copyright (c) 2022
 *
 ************************** Dongguan-University of Technology -ACE************************** */
#include "RemoteControl.h"
#include "SYSConfig.h"

#define Sbus_RX_Buffer_Num 36
#define RC_FRAME_LENGTH 18
#define RC_CHANNAL_ERROR_VALUE 700
//遥控器数据结构体变量
static RC_Ctl_t rc_ctl;
//接收数据缓存，有两个缓冲区，每一缓冲区有18个字节（8*18=144），为防止DMA传输越界给36
uint8_t Sbus_RX_Buffer[2][Sbus_RX_Buffer_Num];




/************************** Dongguan-University of Technology -ACE**************************
 * @brief 获取遥控器数据指针
 * 
 * @return const RC_Ctl_t* 
 ************************** Dongguan-University of Technology -ACE***************************/
const RC_Ctl_t *RC_Get_RC_Pointer(void)
{
    return &rc_ctl;
}


/************************** Dongguan-University of Technology -ACE**************************
 * @brief简介 遥控器初始化函数
 *      先试试以昭廷的dma双缓冲接收遥控器发来的数据，数据帧为18字节
 *      有机会就试试fifo无锁队列
 * @param入参 Rx1_Buff 缓冲区1基地址
 * @param入参 Rx2_Buff 缓冲区2基地址
 * @param入参 Data_Buff_Lenth 数据长度
************************** Dongguan-University of Technology -ACE************************** */
void RC_Init(uint8_t *Rx1_Buff,uint8_t *Rx2_Buff,uint16_t Data_Buff_Lenth)
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
    hdma_usart1_rx.Instance->M0AR=(uint32_t)Rx1_Buff;//设置缓冲区1
    hdma_usart1_rx.Instance->M1AR=(uint32_t)Rx2_Buff;//设置缓冲器2
    hdma_usart1_rx.Instance->NDTR=Data_Buff_Lenth;//设置数据长度

    //使能DMA 双缓存区
    SET_BIT(hdma_usart1_rx.Instance->CR,DMA_SxCR_DBM);

    //使能DMA接收
    __HAL_DMA_ENABLE(&hdma_usart1_rx);
}







/************************** Dongguan-University of Technology -ACE**************************
 * @brief 遥控器数据处理
 * 
 * @param pData 原始数据数组
 * @param RC_CTRL 遥控器统一结构体
 * @return int 
 ************************** Dongguan-University of Technology -ACE***************************/
int RC_DataProcess(volatile const uint8_t *pData,RC_Ctl_t *RC_CTRL)
{
    RC_CTRL->rc.ch0 = ((int16_t)pData[0] | ((int16_t)pData[1] << 8)) & 0x07FF;
    RC_CTRL->rc.ch1 = (((int16_t)pData[1] >> 3) | ((int16_t)pData[2] << 5)) & 0x07FF;
    RC_CTRL->rc.ch2 = (((int16_t)pData[2] >> 6) | ((int16_t)pData[3] << 2) |
                      ((int16_t)pData[4] << 10)) & 0x07FF;
    RC_CTRL->rc.ch3 = (((int16_t)pData[4] >> 1) | ((int16_t)pData[5] << 7)) & 0x07FF;

    RC_CTRL->rc.s1 = ((pData[5] >> 4) & 0x000C) >> 2;
    RC_CTRL->rc.s2 = ((pData[5] >> 4) & 0x000C);

    RC_CTRL->mouse.x = ((int16_t)pData[6]) | ((int16_t)pData[7] << 8);
    RC_CTRL->mouse.y = ((int16_t)pData[8]) | ((int16_t)pData[9] << 8);

    RC_CTRL->mouse.press_l=pData[12];
    RC_CTRL->mouse.press_r=pData[13];

    RC_CTRL->key.v= ((int16_t)pData[14]);// | ((int16_t)pData[15] << 8);

    
 return 0;
}




/************************** Dongguan-University of Technology -ACE**************************
 * @brief 俺自定义的一个串口中断，已经加在stm32f4xx_it.c的用户自定义串口中断里了
 * 
 * @param huart 
 ************************** Dongguan-University of Technology -ACE***************************/
void RC_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    if(huart1.Instance->SR & UART_FLAG_RXNE )
    {
        __HAL_UART_CLEAR_PEFLAG(&huart1);//先对PE位置0，不然读不到数据（？）
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



/************************** Dongguan-University of Technology -ACE**************************
 * @brief 遥控器绝对值处理
 * 
 * @param num 
 * @return int16_t 
 ************************** Dongguan-University of Technology -ACE***************************/
static int16_t RC_abs(int16_t num)
{
    if (num<0)return -num;
    else return num;
}


/************************** Dongguan-University of Technology -ACE**************************
 * @brief 判断遥控器是否出错，采用go to 语句统一将错误数据归零(感觉goto挺好的)
 * 
 * @return uint8_t 
************************** Dongguan-University of Technology -ACE************************** */
uint8_t RC_Check_Data_IS_ERROR(void)
{
    if (RC_abs(rc_ctl.rc.ch0)>RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }
    if (RC_abs(rc_ctl.rc.ch1)>RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }
    if (RC_abs(rc_ctl.rc.ch2)>RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }
    if (RC_abs(rc_ctl.rc.ch3)>RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }
    if (rc_ctl.rc.s1==0)
    {
        goto error;
    }
    if (rc_ctl.rc.s2==0)
    {
        goto error;
    }
    
    return 0;
error:
    rc_ctl.rc.ch0=0;
    rc_ctl.rc.ch1=0;
    rc_ctl.rc.ch2=0;
    rc_ctl.rc.ch3=0;
    

}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 重启遥控器
 * 
 * @param入参 dma_buf_num 
 ************************** Dongguan-University of Technology -ACE***************************/

void RC_Restart(uint16_t dma_buf_num)
{
    __HAL_UART_DISABLE(&huart1);
    __HAL_DMA_DISABLE(&hdma_usart1_rx);

    hdma_usart1_rx.Instance->NDTR=dma_buf_num;  //重新写入长度

    __HAL_UART_ENABLE(&huart1);
    __HAL_DMA_ENABLE(&hdma_usart1_rx);

}




/************************** Dongguan-University of Technology -ACE**************************
 * @brief 遥控数据清零
 * 
 ************************** Dongguan-University of Technology -ACE***************************/
void RC_DataReload(void)
{

    rc_ctl.rc.ch0=0;
    rc_ctl.rc.ch1=0;
    rc_ctl.rc.ch2=0;
    rc_ctl.rc.ch3=0;
    rc_ctl.mouse.x=0;
    rc_ctl.mouse.y=0;
    rc_ctl.mouse.z=0;
    rc_ctl.mouse.press_l=0;
    rc_ctl.mouse.press_r=0;
}