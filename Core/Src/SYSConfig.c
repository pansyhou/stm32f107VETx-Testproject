#include "SYSConfig.h"


static infantry_state_e infantry_state;

/**
  * @brief      设置步兵模式（停止、遥控、键盘）
  * @param[in]  步兵模式
  * @retval     none
  * @attention  
  */
void system_state_set(infantry_state_e state)
{
    infantry_state = state;
}

/**
  * @brief      返回步兵模式（停止、遥控、键盘）
  * @param[in]  none
  * @retval     步兵模式
  * @attention  
  */
infantry_state_e system_state_return(void)
{
    return infantry_state;
}
