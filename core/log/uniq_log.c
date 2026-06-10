#include "uniq_log.h"

/**
 * @brief      硬件接口初始化包装函数
 * 
 * @details    此函数为弱函数，包装硬件接口初始化函数以符合函数指针类型要求，
							 如需使用，用户在自己的源文件中重新定义，并将硬件接口初始化函数
							 在函数内部调用，如<MX_USART1_UART_Init()>等。
 * 
 * @param[in]  param1   	hardware:硬件接口指针，如<&huart1>
 * 
 * @return     返回uniq_log_state_t类型的数值，即enum类型，当前库未严格检查返回状态，
 *						 一律返回<LOG_OK>即可
 *				
 * @note       是否重新定义根据用户自己选择，如使用CUBEMX生成代码，主函数中已经调用
 *						 初始化函数则无需再次重新定义							 
 */
__attribute__((weak)) uniq_log_state_t uniq_hardware_init_wrapper(void* hardware)
{
		return LOG_OK;
}

/**
 * @brief      硬件接口销毁包装函数
 * 
 * @details    此函数为弱函数，包装硬件接口销毁函数以符合函数指针类型要求，
 *						 如需使用，用户在自己的源文件中重新定义，并将硬件接口销毁函数
 *						 在函数内部调用，如<HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)>等。
 * 
 * @param[in]  param1   	hardware:硬件接口指针，如<&huart1>
 * 
 * @return     返回uniq_log_state_t类型的数值，即enum类型，当前库未严格检查返回状态，
 *						 一律返回<LOG_OK>即可
 *				
 * @note       是否重新定义根据用户自己选择，如使用CUBEMX生成代码，销毁函数已有定义
 *						 无需再次重新定义							 
 */
__attribute__((weak)) uniq_log_state_t uniq_hardware_deinit_wrapper(void* hardware)
{
		return LOG_OK;
}

/**
 * @brief      硬件接口发送包装函数
 * 
 * @details    此函数为弱函数，包装硬件接口发送函数以符合函数指针类型要求，
 *						 用户必须在自己的源文件中重新定义，并将硬件接口发送函数在函数内部调用，
 *						 如<HAL_UART_Transmit>等。
 * 
 * @param  param1   	hardware:硬件接口指针，如<&huart1>
 * @param  param2   	pdata:发送缓冲区地址
 * @param  param3   	size:发送字节数
 * @param  param4   	delay:超时时间，如硬件接口使用DMA，则内部调用时无需传入
 * 
 * @return     返回uniq_log_state_t类型的数值，即enum类型，当前库未严格检查返回状态，
 *						 一律返回<LOG_OK>即可
 *				
 * @note       用户必须重新定义该函数，并在内部调用硬件发送函数，否则无法实现打印函数
 *						 与实际硬件发送函数绑定，调用硬件接口发送函数时注意指针类型转换。
 */
__attribute__((weak)) uniq_log_state_t uniq_hardware_transmit_wrapper(void* hardware , const uint8_t* pdata , uint16_t size , uint32_t delay)
{
		//HAL_UART_Transmit((UART_HandleTypeDef*)hardware , pdata , size , delay);
		return LOG_OK;
}

/**
 * @brief      创建一个log设备，将硬件接口与库绑定
 * 
 * @details    创建一个log设备，将结构体绑定包装函数，将硬件接口与log设备绑定，
 *						 首先要定义一个uniq_log_func_t类型的结构体变量，在使用log-API前要先
 *						 调用这个函数。
 * 
 * @param  param1     dev:uniq_log_func_t*类型，将定义的结构体变量地址传进来。
 * @param  param2   	hardware:硬件接口指针
 * 
 * @return     返回uniq_log_func_t类型的指针。
 *						 
 *				
 * @note       用户在使用这个函数前需要先创建一个uniq_log_func_t类型的变量，
 *						 在使用打印或接收函数之前要先调用该函数。
 */
uniq_log_func_t* uniq_create_log_port(uniq_log_func_t* dev , void* hardware)
{
		if(dev == NULL){
				return NULL;
		}
	
		dev->hardware = hardware;
		dev->logInitFunc = uniq_hardware_init_wrapper;
		dev->transmitFunc = uniq_hardware_transmit_wrapper;
		dev->logDeinitFunc = uniq_hardware_deinit_wrapper;
	
		return dev;
}

/**
 * @brief      删除一个log设备
 * 
 * @details    删除现有的log设备，将结构体与硬件接口解绑，调用后结构体变量空间还在，
 *						 但结构体内部为空。
 * 
 * @param  param1   	dev:uniq_log_func_t类型的指针
 * 
 * @return     返回uniq_log_func_t类型的指针，指向被清空的log结构体
 *				
 * @note       调用后log结构体被清空，但分配的内存空间并未消亡。
 */
uniq_log_func_t* uniq_delete_log_port(uniq_log_func_t* dev)
{
		if(dev == NULL){
				return NULL;
		}
	
		memset(dev , NULL , sizeof(uniq_log_func_t));
	
		return dev;
}

/**
 * @brief      log打印函数
 * 
 * @details    此函数为log打印函数，可将数据通过绑定的硬件接口格式化输出。
 * 
 * @param  param1   	dev:uniq_log_func_t类型的指针，即log设备地址
 * @param  param2   	format:字符型指针，此处可以填写字符串和占位符
 * @param  param3   	...:可变参数，根据占位符填写。
 * 
 * @return     返回uniq_log_state_t类型的数值，即enum类型，当前库未严格检查返回状态，
 *						 一律返回<LOG_OK>即可
 *				
 * @note       用户可使用此函数，将数据通过绑定的接口格式化输出到指定位置，如
 *						 串口调试助手中断等。
 */
uniq_log_state_t uniq_log_printf_com(uniq_log_func_t* dev , const char* format , ...)
{
    static char tempBuff[LOG_TX_BUFF_SIZE];
    
    va_list args;
    va_start(args, format);

    int len = vsnprintf(tempBuff , LOG_TX_BUFF_SIZE , format , args);

    va_end(args);

    if(len > 0){
        dev->transmitFunc(dev->hardware , (uint8_t*)tempBuff , len , LOG_TIME_OUT);
    }
}

/**
 * @brief      CRC16校验
 * 
 * 
 * @param  param1   	data:unit8_t*类型，校验数据起始地址
 * @param  param2   	length:uint16_t类型，校验字节数
 * 
 * @return     返回uint16_t类型的CRC校验码
 *						 
 *				
 * @note       返回的CRC结果为大端序，即高位在前，低位在后。
 */
uint16_t uniq_calculate_CRC16(uint8_t* data , uint16_t length)
{   
    uint16_t crc = 0xFFFF;
    for(uint16_t i = 0 ; i < length ; i ++){
        crc ^= data[i];
        for(uint8_t j = 0 ; j < 8 ; j ++){
            if(crc & 1){
                crc >>= 1;
                crc ^= 0xA001;
            }else{
                crc >>= 1;
            }
        }
    }
    return crc;
}
