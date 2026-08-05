#include "bsp_step_s.h"

Step_S_t step;
Motor_Config_t userMotor;

static void Motor_DIR(uint8_t flag)
{
    if(flag){
        HAL_GPIO_WritePin(M2_DIR_GPIO_Port , M2_DIR_Pin , GPIO_PIN_SET);
    }else{
        HAL_GPIO_WritePin(M2_DIR_GPIO_Port , M2_DIR_Pin , GPIO_PIN_RESET);
    }
}

static float Motor_Step_Speed_Calculate(Motor_Config_t* motorx , float speed)
{
    if(NULL == motorx){
        return 0.0f;
    }
    return (speed * motorx->actStepRound / 60.0f);
}

static Step_S_t* Generate_Speed_Table(Step_S_t* stepx)
{
    if(NULL == stepx){
        return NULL;
    }
    for (int i = 0; i < FORM_LEN; i ++) {
        float proportion = (float)i / (float)(FORM_LEN - 1);
        

        float speed = 0.5f * (1.0f - cosf(3.14159265f * proportion));
        // ===============================================
        stepx->speedForm[i] = (uint16_t)(speed * (float)MAX_AUTORELOAD);
    }
    return stepx;
}

Step_S_t* Motor_Step_Init(Step_S_t* stepx , Motor_Config_t* motorx)
{
    if(NULL == stepx){
        
        return NULL;
    }
    memset(stepx , 0 , sizeof(Step_S_t));
    stepx->minSpeed = TIM_FREQ / MAX_AUTORELOAD;
    stepx->vStart = stepx->minSpeed;
    stepx->autoReload = MAX_AUTORELOAD;
    motorx->stepAngle = 1.8f;
    motorx->stepRound = 360.0f / motorx->stepAngle;
    motorx->microStep = 16;
    motorx->actStepRound = motorx->microStep * motorx->stepRound;
    if(NULL == Generate_Speed_Table(stepx)){
        return NULL;
    }
    return stepx;
}

bool Stepper_Move_S(Step_S_t* stepx , Motor_Config_t* motorx , float endSpeed , float accTim , int32_t step) 
{
    if(NULL == stepx || STOP != stepx->stepState){
        return false;
    }
    stepx->vUniform = endSpeed;
    stepx->vUniform = Motor_Step_Speed_Calculate(motorx , stepx->vUniform);
	
    float temp = (stepx->vUniform - stepx->vStart) / accTim;
    float vAvg = (stepx->vUniform + stepx->vStart) / 2.0f;
    int32_t estimatedAccelSteps = (int32_t)(vAvg * accTim);
    if(0 == step){
        
        return false;
    }else if(0 > step){
        Motor_DIR(0);
    }else if(0 < step){
        Motor_DIR(1);
    }
    stepx->totalStep = abs(step);
    
    if (stepx->totalStep >= estimatedAccelSteps * 2) {
        stepx->totalStep = stepx->totalStep;
        stepx->accelTotalTicks = (uint32_t)(accTim * TIM_FREQ);
        stepx->decLocation = stepx->totalStep - estimatedAccelSteps;
        
        BSP_Usart_Printf(&huart2, &port, "S step move!\n");
    } else {
        float S_half = (float)stepx->totalStep / 2.0f; // 一半的位移用于加速
        
        float max_valid_accTim = S_half / vAvg;
        
        // 1. 计算我们设定的全局柔和度系数 K = J/8
        // 根据理论推导，长距离时的 Delta_V = 2 * K * T^2，因此：
        float delta_v_nom = stepx->vUniform - stepx->vStart;
        float K = delta_v_nom / (2.0f * accTim * accTim);
        
        float tNew = 0.0f;
        float vNew = 0.0f;
        if (K < 0.0001f) {
            // 防御性编程：如果是匀速运动，防止除以 0
            tNew = S_half / stepx->vStart;
            vNew = stepx->vStart;
        } else {
            // 2. 求解一元三次方程: K * t^3 + V0 * t - S_half = 0 
            
            // 给牛顿迭代法一个合理的初始猜测值 (假设初速度带来的位移较小，直接取立方根)
            tNew = powf(S_half / K, 1.0f / 3.0f);
            
            // 开始牛顿迭代求精确解 (对于单片机浮点数来说，迭代 3 次精度就极其完美了)
            for(int i = 0; i < 3; i++) {
                // f(t) = K*t^3 + V0*t - S
                float f_t = K * tNew * tNew * tNew + stepx->vStart * tNew - S_half;
                // f'(t) = 3*K*t^2 + V0
                float df_t = 3.0f * K * tNew * tNew + stepx->vStart;
                
                tNew = tNew - (f_t / df_t); // 迭代逼近
            }
            
            // 3. 算出精确的实际运动时间后，反推此时能达到的最高速度
            // 短距离 S 曲线速度增量公式：Delta_V = 2 * K * t^2
            vNew = stepx->vStart + 2.0f * K * tNew * tNew;
            BSP_Usart_Printf(&huart2, &port, "T mode! vNew=%.2f, Max_accTim=%.3fs\n", stepx->vUniform, max_valid_accTim);
        }
        
        // 4. 更新结构体物理参数
        stepx->vUniform = vNew;
        
        // 5. 换算回系统时钟 Tick 数
        stepx->accelTotalTicks = (uint32_t)(tNew * TIM_FREQ);
        
        estimatedAccelSteps = stepx->totalStep / 2;
        stepx->decLocation = stepx->totalStep - estimatedAccelSteps; 
        BSP_Usart_Printf(&huart2, &port, "T step move! vNew = %.2f\n", stepx->vUniform);
    }
    stepx->stepState = ACCEL;
    stepx->location = 0;
    stepx->elapsedTicks = 0;
    stepx->autoReload = (uint16_t)(TIM_FREQ / stepx->vStart);

    HAL_GPIO_WritePin(M2_EN_GPIO_Port , M2_EN_Pin , GPIO_PIN_RESET);
    __HAL_TIM_SET_AUTORELOAD(&htim3 , stepx->autoReload);
    __HAL_TIM_SET_COMPARE(&htim3 , TIM_CHANNEL_3 , stepx->autoReload >> 1);
    __HAL_TIM_SET_COUNTER(&htim3 , 0);
    HAL_TIM_PWM_Start_IT(&htim3 , TIM_CHANNEL_3);
  
    return true;
}

/**
  * @brief  通过查表和线性插值获取当前实时速度
  */
static float GetCurrentSpeed(Step_S_t* stepx) 
{
    if (stepx->elapsedTicks >= stepx->accelTotalTicks) {
        return stepx->vUniform;
    }
    if (stepx->elapsedTicks == 0) {
        return stepx->vStart;
    }

    // 计算时间进度百分比 (0.0 ~ (FORM_LEN - 1))
    float progress = ((float)stepx->elapsedTicks / (float)stepx->accelTotalTicks) * (float)(FORM_LEN - 1);
    
    // 查表与线性插值计算
    uint32_t index = (uint32_t)progress;
    if (index >= FORM_LEN - 1) {
        index = FORM_LEN - 2;
    }
    float fraction = progress - index; // 小数部分
    
    // 提取归一化比例   
    float norm_v = stepx->speedForm[index] + fraction * (stepx->speedForm[index + 1] - stepx->speedForm[index]);
    
    // 映射为真实的物理速度 (Hz)
    return stepx->vStart + (stepx->vUniform - stepx->vStart) * (norm_v / MAX_AUTORELOAD);
}

/**
  * @brief  速度决策逻辑 (在定时器中断中调用)
  */
static void Speed_Decision(Step_S_t* stepx) 
{ 
    float current_speed = 0.0f;
    uint32_t step_ticks = stepx->autoReload; // 当前这1步耗费的定时器周期数

    switch (stepx->stepState) {
        case ACCEL:
            stepx->elapsedTicks += step_ticks;

            // 检查是否完成加速
            if (stepx->elapsedTicks >= stepx->accelTotalTicks) {
                stepx->elapsedTicks = stepx->accelTotalTicks;
                stepx->stepState = UNIFORM;
                
                // 记录真实的加速步数，保证减速与加速对称
                stepx->accelSteps = stepx->location + 1;
                stepx->decLocation = stepx->totalStep - stepx->accelSteps;
            }

            current_speed = GetCurrentSpeed(stepx);
            stepx->autoReload = (uint16_t)(TIM_FREQ / current_speed);
            break;

        case DECEL:
            // 检查是否走完全程
            if (stepx->location >= (stepx->totalStep - 1)) {
                HAL_TIM_OC_Stop_IT(&htim3 , TIM_CHANNEL_3);
                HAL_GPIO_WritePin(M2_EN_GPIO_Port , M2_EN_Pin , GPIO_PIN_SET);
                stepx->stepState = STOP;
                BSP_Usart_Printf(&huart2, &port, "Motor_Move_Over\n");
                break;
            }

            // 反向取速度表
            if (stepx->elapsedTicks > step_ticks) {
                stepx->elapsedTicks -= step_ticks;
            } else {
                stepx->elapsedTicks = 0;
            }

            // 按倒退的时间查表获取速度
            current_speed = GetCurrentSpeed(stepx);
            stepx->autoReload = (uint16_t)(TIM_FREQ / current_speed);
            break;

        case UNIFORM:
            // 监控是否触发减速点
            if (stepx->location >= stepx->decLocation) {
                stepx->stepState = DECEL;
            }
            break;
            
        case STOP:
            break;
    }
    stepx->location ++;
    // 更新定时器比较寄存器
//    uint32_t tim_count = __HAL_TIM_GET_COUNTER(&htim3);
//    uint16_t tmp = (uint16_t)(tim_count + stepx->autoReload);
    __HAL_TIM_SET_AUTORELOAD(&htim3 , stepx->autoReload);
    __HAL_TIM_SET_COMPARE(&htim3 , TIM_CHANNEL_3 , stepx->autoReload >> 1);
}

void Serial_Test_Motor(void)
{
    if(port.rxFlag){
        if(!strncmp((const char*)port.rxBuff, "M1", 2)) {

        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim3){
        Speed_Decision(&step);
    }
}
