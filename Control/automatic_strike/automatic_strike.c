#include "automatic_strike.h"

//视觉变量
Vision_Auto_Data_t Vision_Auto_Data = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0};   // 储存小电脑返回的数据

Vision_Auto_Data_t *Get_Auto_Control_Point(void)
{
    return &Vision_Auto_Data;
}


void MiniPC_Data_Deal(void)
{
    
}