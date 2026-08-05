#ifndef BSP_STEP_S_H
#define BSP_STEP_S_H

#include "main.h"

#define	FORM_LEN            1024																																				
#define MOTOR_ENABLE        1
#define MOTOR_DISABLE		0
#define MOTOR_CW            1
#define MOTOR_CCW           0
#define TIM_FREQ            1000000
#define MAX_AUTORELOAD      30000

typedef struct{
    float stepAngle;				//步进电机步距角
    uint32_t stepRound;				//步进电机初始旋转一圈所需步数
	uint32_t actStepRound;          //步进电机细分后旋转一圈所需步数
	uint16_t microStep;             //步进电机细分数
}Motor_Config_t;

typedef enum{
    STOP = 0,                       //电机停止状态
    ACCEL,                          //电机加速状态
    UNIFORM,                        //电机匀速状态
    DECEL		                    //电机减速状态
}Step_S_State_t;

typedef struct{
    float vStart;                   //电机初始速度（频率单位）等于最小速度
    float vUniform;                 //电机目标速度
    float minSpeed;                 //电机最小速度
    uint32_t accelTotalTicks;       //电机加速度总时间
    uint32_t elapsedTicks;          //电机步间隔时间
    int32_t totalStep;              //电机总运行步数
    int32_t location;               //电机位置即运行过程中步数统计
    int32_t decLocation;            //电机减速位置
    int32_t accelSteps;             //电机加速步数
    uint16_t formLen;               //电机速度表长度
    uint16_t speedForm[FORM_LEN];   //电机速度表
    Step_S_State_t stepState;       //电机运行状态
    uint16_t autoReload;            //电机定时器比较值                       
    uint8_t runDir;                 //电机运行方向
}Step_S_t;

extern Step_S_t step;
extern Motor_Config_t userMotor;

Step_S_t* Motor_Step_Init(Step_S_t* stepx , Motor_Config_t* motorx);
bool Stepper_Move_S(Step_S_t* stepx , Motor_Config_t* motorx , float endSpeed , float accTim , int32_t step);
#endif
