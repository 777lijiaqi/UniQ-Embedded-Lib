bool Generate_Speed_Table(int32_t vUniform , float accel_tim)
{
		float vMiddle = 0.0f;						//中间点速度
		float k = 0.0f;									//加加速度
		float ti = 0.0f;								//时间间隔 dt
		float sumT = 0.0f;							//时间累加量
		float deltaV = 0.0f;						//速度的增量dv
		float temp = 0.0f;							//中间变量
	
		motorSpeed.vUniform = CONVER(vUniform);
	
		accel_tim /= 2.0f;																				//加加速段的时间(加速度斜率>0的时间)
		vMiddle = motorSpeed.vUniform / 2.0f;											//计算中点的速度
		k = fabsf((2.0f * vMiddle) / (accel_tim * accel_tim));		//根据中点速度计算加加速度
		motorSpeed.incAccelTotalStep = (int32_t)((k * accel_tim * accel_tim * accel_tim) / 6.0f);																   //加加速度需要的步数
		accel_tim *= 2.0f;
		motorSpeed.decAccelTotalStep = (int32_t)(((k * accel_tim * accel_tim * accel_tim) / 6.0f) - motorSpeed.incAccelTotalStep); //减加速度需要的步数
		accel_tim /= 2.0f;
		/* 计算共需要的步数 ， 并校验内存大小 ， 申请内存空间存放速度表*/
		motorSpeed.accelTotalStep = motorSpeed.decAccelTotalStep + motorSpeed.incAccelTotalStep;																	 //加速需要的步数
		if(motorSpeed.accelTotalStep % 2 != 0)
		{
				motorSpeed.accelTotalStep += 1;
		}                                                                                                     
		if(FORM_LEN < motorSpeed.accelTotalStep)
		{
				usart_printf(&huart1 , "FORM_LEN 缓存长度不足\r\n , 请将 FORM_LEN修改为%d\r\n" , motorSpeed.accelTotalStep);
				return false;
		}
		
		ti = pow(6.0f * 1.0f / k , 1.0f / 3.0f);
		sumT += ti;
		deltaV = 0.5f * k * sumT * sumT;
		motorSpeed.Form[0] = deltaV;
		
		if(motorSpeed.Form[0] <= MIN_SPEED)
		{
				motorSpeed.Form[0] = MIN_SPEED;
		}
		for(int i = 1 ; i < motorSpeed.accelTotalStep ; i ++)
		{
				ti = 1.0f / motorSpeed.Form[i - 1];
				if(i < motorSpeed.incAccelTotalStep)
				{
						sumT += ti;
						deltaV = 0.5f *	k * sumT * sumT;
						motorSpeed.Form[i] = deltaV;
						if(i == motorSpeed.incAccelTotalStep - 1)
						{
								sumT = fabsf(sumT - accel_tim);
						}
				}else{
						sumT += ti;
						temp = fabsf(accel_tim - sumT);
						deltaV = 0.5f * k * temp * temp;
						motorSpeed.Form[i] = motorSpeed.vUniform - deltaV;
				}
		}
		return true;
}

/**
  * @brief  速度决策
  * @param  pScurve：S曲线结构体指针
  * @retval 无
	*	@note 	在中断中使用，每进一次中断，决策一次
  */
void Speed_Decision(void)
{
		motorState.irqCount ++;
		if(motorState.irqCount == 2)
		{
				motorState.irqCount = 0;
				switch(motorState.state)
				{
						case ACCEL:
												if(motorState.location >= (motorSpeed.accelTotalStep - 1))
												{
														motorState.state = UNIFORM;
														motorState.speedLablePtr -= 1;
														break;
												}
												motorState.compare = (uint16_t)(TIM_FREQ / motorSpeed.Form[motorState.speedLablePtr] / 2.0f);
												motorState.speedLablePtr ++;
						break;
						case DECEL:
												if(motorState.location >= (motorSpeed.totalStep - 1))
												{
														HAL_TIM_OC_Stop_IT(&htim2 , TIM_CHANNEL_2);
														Motor_Turn(MOTOR_DISABLE);
														memset((void*)motorSpeed.Form , 0 , sizeof(float) * FORM_LEN);
														motorState.speedLablePtr = 0;
														motorState.state = STOP;
														if(motorState.runDir == MOTOR_CCW)
														{
																msisc.endFlag = 0;
														}else if(motorState.runDir == MOTOR_CW)
														{
																msisc.startFlag = 0;
														}
														usart_printf(&huart1 , "Motor_Move_Over\n");
														break;
												}
												motorState.compare = (uint16_t)(TIM_FREQ / motorSpeed.Form[motorState.speedLablePtr] / 2.0f);
												motorState.speedLablePtr --;
						break;
						case UNIFORM:
												if(motorState.location >= motorSpeed.decLocation)
												{
														motorState.state = DECEL;
												}
						break;		
				}
				motorState.location ++;
		}
		uint32_t tim_count = __HAL_TIM_GET_COUNTER(&htim2);
		uint16_t tmp = tim_count + motorState.compare;
		__HAL_TIM_SET_COMPARE(&htim2 , TIM_CHANNEL_2 , tmp);
}

/**
  * @brief  步进电机S曲线加减速
	* @param  end_speed：目标速度，单位：转/分钟
	* @param  acc_time：加速时间，单位：秒
	* @param  step：运动步数，单位：步（需考虑细分）
  * @retval true：正常
  * @retval false：参数设置错误或速度表空间不足
	*	@note   无
  */
bool Stepper_Move_S(int16_t end_speed , float acc_time , int32_t step)
{
		if(motorState.state != STOP)
		{
				return false;
		}
		if(Generate_Speed_Table(end_speed , acc_time) != true)
		{
				return false;
		}
		if(step < 0)
		{
				if(msisc.startFlag == 0)
				{
						step = -step;
						Motor_Dir(MOTOR_CCW);														//逆时针向上
				}else{
						usart_printf(&huart1 , "已到达最高点无法继续向上\n");
						//beepFlag = 1;
						return false;
				}
		}else{
				if(msisc.endFlag == 0)
				{
						Motor_Dir(MOTOR_CW);														//顺时针向下
				}else{
						usart_printf(&huart1 , "已到达最低点无法继续向下\n");
						//beepFlag = 2;
						return false;
				}
		}
		if(step >= motorSpeed.accelTotalStep * 2)
		{
				motorSpeed.totalStep = step;
				motorSpeed.decLocation = motorSpeed.totalStep - motorSpeed.accelTotalStep;
		}else{
				usart_printf(&huart1 , "加减速参数设置错误! \r\n");
				return false;
		}
//		memset(&motorState , 0 , sizeof(motorState));
		
		
		motorState.state = ACCEL;
		motorState.location = 0;
		motorState.compare = (uint16_t)(TIM_FREQ / motorSpeed.Form[0]);
		
		__HAL_TIM_SET_COUNTER(&htim2 , 0);
		__HAL_TIM_SET_COMPARE(&htim2 , TIM_CHANNEL_2 , motorState.compare / 2.0f);
		Motor_Turn(MOTOR_ENABLE);
		HAL_TIM_OC_Start_IT(&htim2 , TIM_CHANNEL_2);
		return true;
}


/**
  * @brief	比较输出中断回调函数
  * @param	NULL;
  * @retval	NULL;
  */

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
		if(htim -> Instance == TIM2)
		{
				__HAL_TIM_SetCounter(&htim2 , 0);
				Speed_Decision();
		}
}
