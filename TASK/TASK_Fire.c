#include "TASK_Fire.h"

//变量声明
Fire_task_t Fire_Param;



//通过指针传递信息
const Fire_task_t *Get_Fire_Control_Point(void)
{
    return &Fire_Param;
}