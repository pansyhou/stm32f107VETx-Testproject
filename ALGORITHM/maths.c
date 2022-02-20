#include "maths.h"

int16_t int16_t_abs(int16_t x) // 绝对值计算
{
    if (x < 0)
        x = -x;
    return x;
}