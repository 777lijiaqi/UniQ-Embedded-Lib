#ifndef __UNIQ_LOG_H
#define __UNIQ_LOG_H

#ifndef __MICROLIB
	#error "Library Dependency Error: This library requires MicroLIB! Please check 'Use MicroLIB' in Keil (Options for Target -> Target -> Code Generation)."
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#define LOG_TX_BUFF_SIZE    256								//log设备单次最大发送字节数
#define LOG_TIME_OUT        0xFFFF						//log设备发送超时时间

/**
 * @brief      设备状态枚举
 * @details		 目前该库还未建立完整的状态返回校验机制，该枚举类型
 *						 与HAL库状态枚举类型一一对应
 */
typedef enum{
    LOG_OK,						//log设备正常
    LOG_ERROR,				//log设备错误
    LOG_BUSY,					//log设备忙
		LOG_TIMEOUT				//log设备超时
}uniq_log_state_t;

/**
 * @brief      函数指针类型别名
 */
typedef uniq_log_state_t (*log_init)(void* hardware);								//log设备初始化函数指针类型别名
typedef uniq_log_state_t (*log_deinit)(void* hardware);							//log设备注销函数指针类型别名
typedef uniq_log_state_t (*log_transmit)(void* hardware , const uint8_t* pData , uint16_t size , uint32_t timOut);		//log设备发送函数指针类型别名

/**
 * @brief      log设备接口结构体
 * @details		 内部存放硬件接口API函数				
 */
typedef struct{
    void* hardware;								//硬件接口地址
    log_init logInitFunc;					//硬件接口初始化函数
    log_transmit transmitFunc;		//硬件接口发送函数
    log_deinit logDeinitFunc;			//硬件接口注销函数
}uniq_log_func_t;

uint16_t log_calculate_CRC16(uint8_t* data , uint16_t length);
uniq_log_state_t uniq_hardware_init_wrapper(void* hardware);
uniq_log_state_t uniq_hardware_deinit_wrapper(void* hardware);
uniq_log_state_t uniq_hardware_transmit_wrapper(void* hardware , const uint8_t* pdata , uint16_t size , uint32_t delay);
uniq_log_func_t* uniq_create_log_port(uniq_log_func_t* dev , void* hardware);
uniq_log_state_t uniq_log_printf_com(uniq_log_func_t* dev , const char* format , ...);

#endif
