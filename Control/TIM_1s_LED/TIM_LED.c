#include "TIM_LED.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_0);
    }
}

void PWM_LED(void)
{
    uint16 PWMvalues=0;//占空比
    uint8 dir=0;//方向
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
    while (1)
    {
        if(dir)PWMvalues--;
        else PWMvalues++;
        if(PWMvalues>500-1)dir=1;
        if(PWMvalues==0)dir=0;
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, PWMvalues); 
		HAL_Delay(5);
    }
}