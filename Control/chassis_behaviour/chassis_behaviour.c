#include "chassis_behaviour.h"


/********************底盘电机受控量********************/
static float Chassis_ch0 = 0.0f;    //底盘电机受控量,ch0用于遥控模式的控制量
static float Chassis_ch1 = 0.0f;    //底盘电机受控量
static float Chassis_ch2 = 0.0f;    //底盘电机受控量,ch2用于自动模式的控制量


//底盘遥控选择状态表
Chassis_mode_e Chassis_RC_Mode_Table[RC_SW1_left][RC_SW2_right]=
{
               /*右上*/                     /* 右下 */                          /* 右中 */
  /*左上*/  {CHASSIS_AUTO,              CHASSIS_STANDBY,             CHASSIS_REMOTECONTROL},
  /*左下*/  {CHASSIS_REMOTECONTROL,     CHASSIS_STANDBY,             CHASSIS_REMOTECONTROL},
  /*左中*/  {CHASSIS_AUTO,              CHASSIS_STANDBY,             CHASSIS_REMOTECONTROL}
};



/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘模式设置
 * 
 * @param Chassis_Mode_Set_t 底盘主结构体
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_Behaviour_Mode_Set(Chassis_Control_t *Chassis_Mode_Set_t)
{
  //获取状态后赋值给主结构体
  const char rc_sw1_left  =(Chassis_Mode_Set_t->RC_Chassis_Data->rc.s1 - 1);  //遥控拨杆值-1
  const char rc_sw2_right =(Chassis_Mode_Set_t->RC_Chassis_Data->rc.s2 - 1);
  Chassis_Mode_Set_t->Chassis_Mode = Chassis_RC_Mode_Table[rc_sw1_left][rc_sw2_right];  

  //选择模式
  switch (Chassis_Mode_Set_t->Chassis_Mode)
  {
    //之前为无力状态
    case CHASSIS_STOP://
    {
      RC_DataReload();//遥控器数据清零
      Chassis_Behaviour_Stop(Chassis_Mode_Set_t);
    }break;

    //之前为待机状态
    case CHASSIS_STANDBY:
    {
      Chassis_Mode_Set_t->Chassis_Mode = CHASSIS_INITIALIZE;//状态设置为初始化
      RC_DataReload();     //遥控器数据清零
    }break;

    //自动模式
    case CHASSIS_AUTO:
    {
      Chassis_Behaviour_Auto(Chassis_Mode_Set_t);
    }break;

    //遥控模式
    case CHASSIS_REMOTECONTROL:
    {
      Chassis_Behaviour_RC_Mode(Chassis_Mode_Set_t);
    }break;

    //走位模式
  }
  
  //传入值处理呢？

  
}











/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘无力模式
 * 
 * @param Chassis_Stop_t 底盘主结构体
 ************************** Dongguan-University of Technology -ACE***************************/
static void Chassis_Behaviour_Stop(Chassis_Control_t *Chassis_Stop_t)
{
  //全部四个底盘电机输出为0
  Chassis_Stop_t->motor[0].output=0;
  Chassis_Stop_t->motor[1].output=0;
  Chassis_Stop_t->motor[2].output=0;
  Chassis_Stop_t->motor[3].output=0;

  //控制量为0
  Chassis_ch0=Chassis_ch1=Chassis_ch2=0;
}



/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘自动模式
 * 
 * @param Chassis_Auto_t 底盘主结构体
 ************************** Dongguan-University of Technology -ACE***************************/
static void Chassis_Behaviour_Auto(Chassis_Control_t *Chassis_Auto_t)
{
  Chassis_ch2 = CHASSIS_AUTO_SPPED;
}


/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘遥控模式
 * 
 * @param Chassis_RC_Mode_t 底盘主结构体
 ************************** Dongguan-University of Technology -ACE***************************/
static void Chassis_Behaviour_RC_Mode(Chassis_Control_t *Chassis_RC_Mode_t)
{
  Chassis_ch0=Chassis_RC_Mode_t->RC_Chassis_Data->rc.ch0;
}