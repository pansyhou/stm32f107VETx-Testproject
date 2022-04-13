#ifndef __CHASSIS_BEHAVIOUR_H
#define __CHASSIS_BEHAVIOUR_H

#include "SYSConfig.h"

#define RC_SW1_left 3  //遥控器发射机左S1开关位置数
#define RC_SW2_right 3 //遥控器发射机右S2开关位置数

typedef enum
{
    CHASSIS_STOP,       //停止
    CHASSIS_INITIALIZE, //初始化中
    CHASSIS_STANDBY,    //待机

    CHASSIS_FOLLOW,    //跟随
    CHASSIS_NO_FOLLOW, //不跟随

    CHASSIS_TWIST_WAIST, //扭腰
    CHASSIS_ROTATION,    //小陀螺
    CHASSIS_BATTERY,     //炮台模式

    CHASSIS_REMOTECONTROL, //遥控模式
    CHASSIS_AUTO,          //自动模式
    CHASSIS_BLOCKING,      //走位模式
    CHASSIS_FIXATION       // 底盘固定模式
}Chassis_mode_e;



/******************函数声明*******************/

void Chassis_Behaviour_Mode_Set(Chassis_Control_t *Chassis_Mode_Set_t);//底盘模式设置
static void Chassis_Behaviour_Stop(Chassis_Control_t *Chassis_Stop_t);//底盘无力模式
static void Chassis_Behaviour_Auto(Chassis_Control_t *Chassis_Auto_t);//底盘自动模式    
static void Chassis_Behaviour_RC_Mode(Chassis_Control_t *Chassis_RC_Mode_t);//底盘遥控器模式
static void chassis_mode_choose(Chassis_Control_t *chassis_mode_choose_f);//底盘模式选择


#endif