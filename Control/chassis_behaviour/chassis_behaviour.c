#include "chassis_behaviour.h"

/********************底盘电机受控量********************/
static float Chassis_ch0 = 0.0f; //底盘电机受控量,ch0用于遥控模式的控制量
static float Chassis_ch1 = 0.0f; //底盘电机受控量
static float Chassis_ch2 = 0.0f; //底盘电机受控量,ch2用于自动模式的控制量

// //底盘遥控选择状态表
// Chassis_mode_e Chassis_RC_Mode_Table[RC_SW1_left][RC_SW2_right]=
// {
//                /*右上*/                     /* 右下 */                          /* 右中 */
//   /*左上*/  {CHASSIS_AUTO,              CHASSIS_STANDBY,             CHASSIS_REMOTECONTROL},
//   /*左下*/  {CHASSIS_REMOTECONTROL,     CHASSIS_STANDBY,             CHASSIS_REMOTECONTROL},
//   /*左中*/  {CHASSIS_AUTO,              CHASSIS_STANDBY,             CHASSIS_REMOTECONTROL}
// };

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘模式设置
 *
 * @param Chassis_Mode_Set_t 底盘主结构体
 ************************** Dongguan-University of Technology -ACE***************************/
void Chassis_Behaviour_Mode_Set(Chassis_Control_t *Chassis_Mode_Set_t)
{

  //获取状态后赋值给主结构体
  // const char rc_sw1_left  =(Chassis_Mode_Set_t->RC_Chassis_Data->rc.s1 - 1);  //遥控拨杆值-1
  // const char rc_sw2_right =(Chassis_Mode_Set_t->RC_Chassis_Data->rc.s2 - 1);
  // Chassis_Mode_Set_t->Chassis_Mode = Chassis_RC_Mode_Table[rc_sw1_left][rc_sw2_right];  //哨兵家的

  //选择模式
  chassis_mode_choose(Chassis_Mode_Set_t);
  if((Chassis_Mode_Set_t->gimbal_re_data->Gimbal_all_flag==1)&&(system_state_return() != INF_STOP))
  {
    //这段pid设置好像还暂时不会用到
    // chassis_behaviour_f->chassis_speed_pid[0].pid_mode = PID_NORMAL;
    // chassis_behaviour_f->chassis_speed_pid[1].pid_mode = PID_NORMAL;
    // chassis_behaviour_f->chassis_speed_pid[2].pid_mode = PID_NORMAL;
    // chassis_behaviour_f->chassis_speed_pid[3].pid_mode = PID_NORMAL;
    if(system_state_return() == INF_MK)
    {
      switch (Chassis_Mode_Set_t->Chassis_Mode)
      {
        case CHASSIS_FOLLOW://跟随
        {
          chassis_follow(Chassis_Mode_Set_t);//TODO:底盘行为各个函数没写

        }break;

        case CHASSIS_NO_FOLLOW://不跟随
        {
          chassis_independent(Chassis_Mode_Set_t);
        }break;

        case CHASSIS_BATTERY://*炮台模式（底盘位置环锁死
        {
          Chassis_ch0 = Chassis_ch1 = Chassis_ch2 = 0.0f;
        }break;

        case CHASSIS_TWIST_WAIST://扭腰
        {
          chassis_twist(Chassis_Mode_Set_t, Chassis_Mode_Set_t->key.key_w + Chassis_Mode_Set_t->key.key_s, Chassis_Mode_Set_t->key.key_a + Chassis_Mode_Set_t->key.key_d);
        }break;

        case CHASSIS_ROTATION://小陀螺
        {
          chassis_rotation(Chassis_Mode_Set_t, Chassis_Mode_Set_t->key.key_w + Chassis_Mode_Set_t->key.key_s, Chassis_Mode_Set_t->key.key_a + Chassis_Mode_Set_t->key.key_d);
        }break;
      }//switch终点
    }
    else 
    {
      switch (Chassis_Mode_Set_t->Chassis_Mode)
      {
        case CHASSIS_FOLLOW://跟随
        {
          chassis_follow(Chassis_Mode_Set_t);
        }break;
        case CHASSIS_NO_FOLLOW:
        {
          chassis_independent(Chassis_Mode_Set_t);
        }break;
        case CHASSIS_TWIST_WAIST:
        {
          chassis_twist(Chassis_Mode_Set_t, Chassis_Mode_Set_t->RC_Chassis_Data->rc.ch[1], Chassis_Mode_Set_t->RC_Chassis_Data->rc.ch[0]);
        }break;
        case CHASSIS_ROTATION:
        {
          chassis_rotation(Chassis_Mode_Set_t, Chassis_Mode_Set_t->RC_Chassis_Data->rc.ch[1], Chassis_Mode_Set_t->RC_Chassis_Data->rc.ch[0]);
        }break;
      }
      //传入值处理
      Chassis_set_remote(Chassis_Mode_Set_t, Chassis_ch0, Chassis_ch1, Chassis_ch2);
    }
  }
  else 
  {
    //pid设置为手动模式
    // chassis_behaviour_f->chassis_speed_pid[0].pid_mode = PID_MANUAL;
    // chassis_behaviour_f->chassis_speed_pid[1].pid_mode = PID_MANUAL;
    // chassis_behaviour_f->chassis_speed_pid[2].pid_mode = PID_MANUAL;
    // chassis_behaviour_f->chassis_speed_pid[3].pid_mode = PID_MANUAL;
    Chassis_Task_OFF(0);
    Chassis_set_remote(Chassis_Mode_Set_t, 0.0f, 0.0f, 0.0f);
    RC_DataReload(); //摇杆量清零
  }
}


/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘无力模式
 *
 * @param Chassis_Stop_t 底盘主结构体
 ************************** Dongguan-University of Technology -ACE***************************/
static void Chassis_Behaviour_Stop(Chassis_Control_t *Chassis_Stop_t)
{
  //全部四个底盘电机输出为0
  Chassis_Stop_t->motor[0].output = 0;
  Chassis_Stop_t->motor[1].output = 0;
  Chassis_Stop_t->motor[2].output = 0;
  Chassis_Stop_t->motor[3].output = 0;

  //控制量为0
  Chassis_ch0 = Chassis_ch1 = Chassis_ch2 = 0;
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
  Chassis_ch0 = Chassis_RC_Mode_t->RC_Chassis_Data->rc.ch[0];
}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 底盘模式选择
 * 
 * @param chassis_mode_choose_f 
 ************************** Dongguan-University of Technology -ACE***************************/
static void chassis_mode_choose(Chassis_Control_t *chassis_mode_choose_f)
{
  
  switch (chassis_mode_choose_f->RC_Chassis_Data->rc.s2)
  {
    /* 右开关打上，遥控控制 */
    case RC_SW_UP:
    {
      if (chassis_mode_choose_f->gimbal_re_data->Gimbal_all_flag == 1) //云台已经初始化
      {
        system_state_set(INF_RC);

        switch (chassis_mode_choose_f->RC_Chassis_Data->rc.s1)
        {
          case RC_SW_UP://s1（左上）打到上
          {
            chassis_mode_choose_f->Chassis_Mode = CHASSIS_ROTATION; //底盘小陀螺模式 S1上S2上
          }break;

          case RC_SW_MID://s1（左上）打到中间
          {
            chassis_mode_choose_f->Chassis_Mode =CHASSIS_FOLLOW;//底盘跟随模式
          }break;
          
          case RC_SW_DOWN://s1（左上）打到最下
          {
            chassis_mode_choose_f->Chassis_Mode = CHASSIS_NO_FOLLOW;//底盘不跟随
          }break;
        }
      }
    }break;
    /* 右开关打中，键盘控制 */
    case RC_SW_MID:
    {
      if (chassis_mode_choose_f->gimbal_re_data->Gimbal_all_flag == 1) //云台已经初始化
      {
        switch (chassis_mode_choose_f->RC_Chassis_Data->rc.s1)
        {
          //开关切换模式
          case RC_SW_UP://s1（左上）打到上
          {
            system_state_set(INF_RC);
          }break;
          
          case RC_SW_MID://s1（左上）打到中间
          {
            if (system_state_return() != INF_MK)
            {
                chassis_mode_choose_f = CHASSIS_FOLLOW; //状态设置为跟随
            }
            system_state_set(INF_MK);
            mk_data_process(chassis_mode_choose_f); //!键盘模式
          }break;

          case RC_SW_DOWN://s1（左上）打到最下
          {
            chassis_mode_choose_f = CHASSIS_BATTERY; //炮台模式
            system_state_set(INF_RC);
          }break;
        }
        
      }
    }break;

    /* 右开关打下，停止工作 */
    case RC_SW_DOWN:
    {
      chassis_mode_choose_f = CHASSIS_STOP;
      system_state_set(INF_STOP); //进入停止状态
    }break;

    /* 出现严重错误 */
    case RC_SW_ERROR:
    {
      chassis_mode_choose_f=CHASSIS_STANDBY;//进入待机模式
    }break;
  }
}



/**
  * @brief          底盘跟随状态控制
  * @param[in]      *Chassis_Follow_f：底盘主结构体
  * @retval         none\
  * //TODO:底盘跟随状态控制还没测试 & 我也没懂他的原理是怎么跟随的，跟随的是鼠标键盘遥控器那些吗？
  */

static void chassis_follow(Chassis_Control_t *Chassis_Follow_f)
{
	if (system_state_return() == INF_RC)
	{
		Chassis_ch0 = Chassis_Follow_f->RC_Chassis_Data->rc.ch[0];
		Chassis_ch1 = Chassis_Follow_f->RC_Chassis_Data->rc.ch[1];
	}
	else if (system_state_return() == INF_MK)
	{
		Chassis_ch0 = Chassis_Follow_f->key.key_a + Chassis_Follow_f->key.key_d;
		Chassis_ch1 = Chassis_Follow_f->key.key_w + Chassis_Follow_f->key.key_s;
	}
		//获取云台与底盘的差角
		Chassis_ch2 = Chassis_Follow_f->Angel_Between_Chassis_Gimbal;
		
		//将跟随底盘的pid目标值设定为差角
		Chassis_Follow_f->chassis_rotate_pid.SetValue = Chassis_ch2;
		//将经过pid计算的输出值传给 Chassis_ch2
		Chassis_ch2 = location_pid_int32(&Chassis_Follow_f->chassis_rotate_pid, 0.0f);
		Chassis_ch2 = int32_limit(Chassis_ch2, 1000, -1000); //输出限制
}




/**
  * @brief          底盘不跟随状态控制
  * @param[in]      *Chassis_Independent_f：底盘主结构体
  * @retval         none
  */
static void chassis_independent(Chassis_Control_t *Chassis_Independent_f)
{
	if (system_state_return() == INF_RC)
	{
		/* 小陀螺旋转移动算法：云台为主坐标轴，目标值分解到底盘坐标轴 */
		if (Chassis_Independent_f->RC_Chassis_Data->rc.ch[0] || Chassis_Independent_f->RC_Chassis_Data->rc.ch[1]) //此时有移动    custom_sin  sin_calculate()
		{
			//以步兵建立坐标轴，左负右正
			Chassis_ch2 = 0;//TODO:云台与底盘差角在哪更新计算？

			Chassis_ch1 = cos_calculate(Chassis_Independent_f->Angel_Between_Chassis_Gimbal) * Chassis_Independent_f->RC_Chassis_Data->rc.ch[1];
			Chassis_ch0 = sin_calculate(Chassis_Independent_f->Angel_Between_Chassis_Gimbal) * Chassis_Independent_f->RC_Chassis_Data->rc.ch[1];

			if (Chassis_Independent_f->RC_Chassis_Data->rc.ch[0])
			{
				Chassis_ch1 += -sin_calculate(Chassis_Independent_f->Angel_Between_Chassis_Gimbal) * Chassis_Independent_f->RC_Chassis_Data->rc.ch[0];
				Chassis_ch0 += cos_calculate(Chassis_Independent_f->Angel_Between_Chassis_Gimbal) * Chassis_Independent_f->RC_Chassis_Data->rc.ch[0];
			}
		}
		else
		{
			Chassis_ch0 = 0;
			Chassis_ch1 = 0;
		}		
	}
	else if (system_state_return() == INF_MK)
	{
		Chassis_ch2 = 0;

		Chassis_ch1 = cos_calculate(Chassis_Independent_f->Angel_Between_Chassis_Gimbal) * (Chassis_Independent_f->key.key_w + Chassis_Independent_f->key.key_s);
		Chassis_ch0 = sin_calculate(Chassis_Independent_f->Angel_Between_Chassis_Gimbal) * (Chassis_Independent_f->key.key_w + Chassis_Independent_f->key.key_s);

		Chassis_ch1 += -sin_calculate(Chassis_Independent_f->Angel_Between_Chassis_Gimbal) * (Chassis_Independent_f->key.key_a + Chassis_Independent_f->key.key_d);
		Chassis_ch0 += cos_calculate(Chassis_Independent_f->Angel_Between_Chassis_Gimbal) * (Chassis_Independent_f->key.key_a + Chassis_Independent_f->key.key_d);
	}

#if CHASSIS_TEST_MODE
    chassis_cos_calculate_jscope = cos_calculate(Chassis_Independent_f->chassis_gimbal_angel) * 100;
    chassis_sin_calculate_jscope = sin_calculate(Chassis_Independent_f->chassis_gimbal_angel) * 100;
#endif
}


//TODO:扭腰算法理论部分补充
static int16_t TWIST_Flag = 0;        //0,左转；1，右转
static int16_t TWIST_Chassis_acc = 0; //底盘扭腰加速度限制
/**
  * @brief          底盘扭腰状态控制
  * @param[in]      *Chassis_Twist_f：底盘主结构体
  * @retval         none
  */
static void chassis_twist(Chassis_Control_t *Chassis_Twist_f, fp32 Before_After, fp32 Left_Right)
{
    //*判断转向
    if (Chassis_Twist_f->Angel_Between_Chassis_Gimbal > 20 && TWIST_Flag == 1)
    {
        TWIST_Flag = 0;
    }
    else if (Chassis_Twist_f->Angel_Between_Chassis_Gimbal < -20 && TWIST_Flag == 0)
    {
        TWIST_Flag = 1;
    }

    //*转动控制
    if (TWIST_Flag == 0) //左角度大于30，向左转
    {
        //*开环固定扭速
        if (TWIST_Chassis_acc <= CHASSIS_TWIST_SPEED)
            TWIST_Chassis_acc += 20;
        Chassis_ch2 = TWIST_Chassis_acc;

        //*扭腰移动算法：云台为主坐标轴，目标值分解到底盘坐标轴
        if (Before_After || Left_Right)
        {
            Chassis_ch1 = cos_calculate(Chassis_Twist_f->Angel_Between_Chassis_Gimbal) * Before_After;
            Chassis_ch0 = sin_calculate(Chassis_Twist_f->Angel_Between_Chassis_Gimbal) * Before_After;

            Chassis_ch1 += -sin_calculate(Chassis_Twist_f->Angel_Between_Chassis_Gimbal) * Left_Right;
            Chassis_ch0 += cos_calculate(Chassis_Twist_f->Angel_Between_Chassis_Gimbal) * Left_Right;
        }
        else
        {
            Chassis_ch0 = 0;
            Chassis_ch1 = 0;
        }
    }
    else if (TWIST_Flag == 1) //右角度小于负30，向右转
    {
        //*开环固定扭速
        if (TWIST_Chassis_acc >= -CHASSIS_TWIST_SPEED)
            TWIST_Chassis_acc -= 20;
        Chassis_ch2 = TWIST_Chassis_acc;

        //*扭腰移动算法：云台为主坐标轴，目标值分解到底盘坐标轴
        if (Before_After || Left_Right)
        {
            Chassis_ch1 = cos_calculate(Chassis_Twist_f->Angel_Between_Chassis_Gimbal) * Before_After;
            Chassis_ch0 = sin_calculate(Chassis_Twist_f->Angel_Between_Chassis_Gimbal) * Before_After;

            Chassis_ch1 += -sin_calculate(Chassis_Twist_f->Angel_Between_Chassis_Gimbal) * Left_Right;
            Chassis_ch0 += cos_calculate(Chassis_Twist_f->Angel_Between_Chassis_Gimbal) * Left_Right;
        }
        else
        {
            Chassis_ch0 = 0;
            Chassis_ch1 = 0;
        }
    }
    else
    {
        Chassis_ch0 = 0;
        Chassis_ch1 = 0;
        Chassis_ch2 = 0;
    }
}



fp32 obtain_rotate_motor_gain(void)
{
    uint16_t power = referee_chassis_power_limit();

    if (power == 40)
    {
        return 3.0f;
    }
    else if (power == 45)
    {
        return 3.5f;
    }
    else if (power == 50)
    {
        return 4.5f;
    }
    else if (power == 55)
    {
        return 4.5f;
    }
    else if (power == 60)
    {
        return 5.0f;
    }
    else if (power == 80)
    {
        return 7.0f;
    }
    else if (power == 100)
    {
        return 8.0f;
    }
    else
    {
        return 7.0f;
    }
}

/**
  * @brief          底盘小陀螺状态控制
  * @param[in]      *Chassis_Rotation_f：底盘主结构体
  * @retval         none
  */
static void chassis_rotation(Chassis_Control_t *Chassis_Rotation_f, fp32 Before_After, fp32 Left_Right)
{
    fp32 gain = obtain_rotate_motor_gain(); //TODO:这啥玩意？

    //*小陀螺旋转移动算法：云台为主坐标轴，目标值分解到底盘坐标轴
    if (Before_After || Left_Right) //此时有移动    custom_sin  sin_calculate()
    {
        //*以步兵建立坐标轴，左负右正
        Chassis_ch2 = CHASSIS_ROTATION_SPEED * 0.9f * gain; //小陀螺移动时减速 0.8

        Chassis_ch1 = cos_calculate(Chassis_Rotation_f->Angel_Between_Chassis_Gimbal) * Before_After;
        Chassis_ch0 = sin_calculate(Chassis_Rotation_f->Angel_Between_Chassis_Gimbal) * Before_After;

        if (Left_Right)
        {
            Chassis_ch1 += -sin_calculate(Chassis_Rotation_f->Angel_Between_Chassis_Gimbal) * Left_Right;
            Chassis_ch0 += cos_calculate(Chassis_Rotation_f->Angel_Between_Chassis_Gimbal) * Left_Right;
        }
    }
    else //原地转快速
    {
        //*开环固定转速
        Chassis_ch2 = CHASSIS_ROTATION_SPEED * gain; //小陀螺移动时减速

        Chassis_ch0 = 0;
        Chassis_ch1 = 0;
    }
}


/**
  * @brief          底盘无力
  * @param[in]      *Chassis_Stop_f：底盘主结构体
  * @retval         none
  */
void Chassis_Stop(Chassis_Control_t *Chassis_Stop_f)
{
    //*控制量
    Chassis_ch0 = Chassis_ch1 = Chassis_ch2 = 0.0f;
}
