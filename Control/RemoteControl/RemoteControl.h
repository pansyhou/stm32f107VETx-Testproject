/************************** Dongguan-University of Technology -ACE**************************
 * @file RemoteControl.h
 * @author pansyhou侯文辉 (1677195845lyb@gmail.com)
 * @brief简介 遥控器相关头文件
 * @version 0.1
 * @date 2022-01-26
 *
 * @copyright Copyright (c) 2022
 *
 ************************** Dongguan-University of Technology -ACE************************** */

#ifndef  __REMOTECONTROL_H
#define  __REMOTECONTROL_H

/*
 * 遥控器接收数据结构体*/
 //__packed为字节对齐
typedef __packed struct
{
    __packed struct
    {
        uint16_t ch0;
        uint16_t ch1;
        uint16_t ch2;
        uint16_t ch3;
        uint8_t s1;
        uint8_t s2;
    } rc;
    __packed struct
    {
        int16_t x;
        int16_t y;
        int16_t z;
        uint8_t press_l;
        uint8_t press_r;
    } mouse;
    __packed struct
    {
        uint16_t v;
    } key;
} RC_Ctl_t;



void    RC_UART_IRQHandler(UART_HandleTypeDef *huart);
int     RC_DataProcess(volatile const uint8_t *pData,RC_Ctl_t *RC_CTRL);
void    RC_Init(uint8_t *Rx1_Buff,uint8_t *Rx2_Buff,uint16_t Data_Buff_Lenth);
const   RC_Ctl_t *RC_Get_RC_Pointer(void);
static  int16_t RC_abs(int16_t num);
uint8_t RC_Check_Data_IS_ERROR(void);
void    RC_Restart(uint16_t dma_buf_num);
void    RC_DataReload(void);
#endif

