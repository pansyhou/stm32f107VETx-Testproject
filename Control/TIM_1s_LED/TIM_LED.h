#ifndef __TIM_LED_H
#define __TIM_LED_H
#include "main.h"
#include "tim.h"
void PWM_LED(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#endif

