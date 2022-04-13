#ifndef TASK_FIRE_H
#define TASK_FIRE_H

#include "SYSConfig.h"

/*OS控制任务周期以及启动时间*/
#define FIRE_TASK_INIT_TIME  5
#define FIRE_CONTROL_TIME_MS 2

#define REVERES_LIGHT_COUPLING  0 					// 是否使用反向光偶   1 -> 使用 0 -> 未使用
#define	LOADING_STOP_TIMES  700                     // 堵转计时

/*OS控制任务周期以及启动时间*/
#define FIRE_TASK_INIT_TIME  5
#define FIRE_CONTROL_TIME_MS 2

/*摩擦轮*/
#define PWM_Shoot_Left   TIM4->CCR1  //PD12
#define PWM_Shoot_Right  TIM4->CCR2	 //PD13

/**********拨弹电机速度环pid参数定义**********/
#define FIRE_S_P 15.0f   //供弹电机M2006速度环
#define FIRE_S_I 0.2f
#define FIRE_S_D 2.0f

/**********发弹系统速度设定**********/
#if (INFANTRY_NUM == INFANTRY_4)
//零级：15m/s   一级：18m/s  二级：22m/s  三级：30m/s
#define FRICTION_THREE_SHOOT_SPEED     1450       //30摩擦轮高速pwm
#define FRICTION_TWO_SHOOT_SPEED       1343       //22摩擦轮高速pwm
#define FRICTION_ONE_SHOOT_SPEED       1298       //18摩擦轮高速pwm
#define FRICTION_ZERO_SHOOT_SPEED      1260       //15摩擦轮低速pwm    
#define FRICTION_SHOOT_STOP            1000       //0摩擦轮停止pwm
#elif (INFANTRY_NUM == INFANTRY_3)
//零级：15m/s   一级：18m/s  二级：22m/s  三级：30m/s
#define FRICTION_THREE_SHOOT_SPEED     1460       //30摩擦轮高速pwm
#define FRICTION_TWO_SHOOT_SPEED       1363       //22摩擦轮高速pwm
#define FRICTION_ONE_SHOOT_SPEED       1292       //18摩擦轮高速pwm
#define FRICTION_ZERO_SHOOT_SPEED      1270       //15摩擦轮低速pwm    
#define FRICTION_SHOOT_STOP            1000       //0摩擦轮停止pwm
#endif

#define LOADING_SPEED_H                (4000)     //供弹电机速度 3000    2500
#define LOADING_SPEED_L                (2500)     //供弹电机速度 3000    2500

//卡弹时间 以及反转时间
#define BLOCK_TRIGGER_SPEED         100.0f
#define BLOCK_TIME                  700  //700
#define REVERSE_TIME                250


/*发弹模式*/
typedef enum
{
    FIRE_H = 0,    //高射频
	FIRE_L,        //低射频
    AUTO_FIRE,     //自动发射
    STOP_FIRE,     //停止发射
    BACK,          //退弹
	FIRE_ERROR,
} fire_work_status_e;

/*摩擦轮模式*/
typedef enum
{
    LOW_SPEED,
    HIGH_SPEED,
    STOP_SHOOT,
	SHOOT_ERROR,
} shoot_work_status_e;



/*火控结构体*/
/*火控结构体*/
typedef struct Fire_param
{
    const CAN_Motor_Measure_Data *fire_motor_measure;
    const RC_Ctl_t *fire_RC;   //开火任务使用的遥控器指针
    PIDTypeDef fire_s_pid;      //拨弹2006电机pid

    fire_work_status_e fire_mode;   //射弹模式
	fire_work_status_e last_fire_mode;   //射弹模式
    shoot_work_status_e friction_mode;  //摩擦轮模式

	uint8_t fire_switch;    //拨弹开关
    int16_t GD_output;      //供弹电机输出
	int16_t GD_set_speed;   //供弹电机速度
    int shoot_speed;        //当前设置射速
    //uint8_t dead_mode;      //死亡模式开关
	
	uint16_t hot_max;      //最大热量
	uint16_t hot_current;  //当前热量
	
	fp32 GD_speed_gain;   //用于热量限制
	
    uint16_t block_time;    //堵转时间
    uint16_t reverse_time;  //回转时间

} fire_task_t;


const fire_task_t *Get_Fire_Control_Point(void);

#endif // DEBUG