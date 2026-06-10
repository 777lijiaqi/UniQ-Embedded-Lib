#define	FORM_LEN				15000																			//速度表空间大小
#define TIM_FREQ				1000000																		//频率ft值
#define STEP_ANGLE  		1.8f																			//步进电机的步距角 单位：度
#define FSPR						(360.0f / 1.8f)														//步进电机的一圈所需脉冲数
#define MICRO_STEP			16																				//细分器细分数
#define SPR							(FSPR * MICRO_STEP)												//细分后一圈所需脉冲数
#define CONVER(speed)		(float)(speed * SPR / 60.0f)							//根据电机转速(r / min) , 计算电机步速(step / s)
#define MIN_SPEED				(float)(TIM_FREQ / 65535)									//最低频率 / 速度
#define MOTOR_ENABLE		1
#define MOTOR_DISABLE		0
#define MOTOR_CW				0
#define MOTOR_CCW				1

typedef struct{
		__IO uint8_t state;							//状态
		__IO uint16_t compare;					//比较值
		__IO uint32_t location;					//位置
		__IO uint8_t irqCount;					//中断计数器
		__IO uint32_t speedLablePtr;		//速度表索引指示器
		__IO uint8_t runDir;						//运行方向
}motorState_Typedef;

typedef struct{
		__IO int32_t	vStart;						//初始速度
		__IO int32_t	vUniform;					//目标速度
		__IO int32_t  accelTotalStep;		//加速度总步数
		__IO int32_t  decLocation;			//开始减速的位置
		__IO int32_t  totalStep;				//完整曲线总步数
		__IO int32_t  incAccelTotalStep;//加加速度步数
		__IO int32_t  decAccelTotalStep;//减加速度步数
		__IO float	Form[FORM_LEN];			//S加减速 速度表
}motorSpeed_Typedef;

typedef enum{
		STOP = 0,												//停止状态
		ACCEL,													//加速状态
		UNIFORM,												//匀速状态
		DECEL,													//减速状态
}motorStateEnum_Typedef;


extern motorSpeed_Typedef motorSpeed;
extern motorState_Typedef motorState;


/**
  * @brief  电机使能/失能
  * @param  参数
  * @retval 无
	*	@note 	无
  */
__STATIC_INLINE void Motor_Turn(uint8_t flag)
{
		if(flag)
		{
				HAL_GPIO_WritePin(M_EN_GPIO_Port , M_EN_Pin , GPIO_PIN_RESET);
		}else{
				HAL_GPIO_WritePin(M_EN_GPIO_Port , M_EN_Pin , GPIO_PIN_SET);
		}
}

/**
  * @brief  电机顺/逆时针旋转
	* @param  参数一:方向，0为顺时针旋转 ， 1为逆时针旋转
  * @retval 无
	*	@note 	无
  */
__STATIC_INLINE void Motor_Dir(uint8_t dir)
{
		if(dir == MOTOR_CCW)
		{
				HAL_GPIO_WritePin(M_DIR_GPIO_Port , M_DIR_Pin , GPIO_PIN_SET);
				motorState.runDir = MOTOR_CCW;		
		}else if(dir == MOTOR_CW){
				HAL_GPIO_WritePin(M_DIR_GPIO_Port , M_DIR_Pin , GPIO_PIN_RESET);
				motorState.runDir = MOTOR_CW;
		}
}

void motor_init(void);
bool Generate_Speed_Table(int32_t vUniform , float accel_tim);
void Speed_Decision(void);
bool Stepper_Move_S(int16_t end_speed , float acc_time , int32_t step);
