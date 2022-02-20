#include "gimbal_app.h"
#include "SYSConfig.h"

osThreadId GimbalTaskHandle;
static CAN_TxHeaderTypeDef Txmessage;   //发送CAN信息结构体

extern TIM_HandleTypeDef htim1;


//这里有段PWM输出，我也不知道是什么，暂时不写

void Gimbal_app_Init(void)
{
    //这里又来一段PWM，保守估计是灯？或者是os的时基

    osThreadDef(GimbalTask,Gimbal_Task,osPriorityRealtime,0,200);//栈为200
    GimbalTaskHandle = osThreadCreate(osThread(GimbalTask));

}
