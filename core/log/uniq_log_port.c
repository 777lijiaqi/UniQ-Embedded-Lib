#include "uniq_log.h"

/*
    发送接口包装函数，适配函数指针，将硬件层发送函数填入函数中
*/
static logState_t stm32_usart_transmit_wrapping(void* hardware , const uint8_t* pData , uint16_t size , uint32_t timOut)
{
    /*此处填写硬件层发送函数，例：HAL_UART_Transmit((UART_HandleTypedef*)hardware , (uint8_t*)pData , size , timOut);
    如果是使用DMA发送的方式可以写为：HAL_UART_Transmit_DMA((UART_HandleTypedef*)hardware , (uint8_t*)pData , size);
    */

    return LOG_OK;
}

/*
    接收接口包装函数，适配函数指针，将硬件层接收函数填入函数中
*/
static logState_t stm32_usart_receive_wrapping(void* hardware , const uint8_t* pData , uint16_t size , uint32_t timOut)
{
    /*此处填写硬件层接收函数，例：HAL_UART_Receive((UART_HandleTypedef*)hardware , (uint8_t*)pData , size , timOut);
    如果是使用DMA发送+空闲中断的方式可以写为：HAL_UART_ReceiveTodoIDLE_DMA((UART_HandleTypedef*)hardware , (uint8_t*)pData , size);
    */
   
    return LOG_OK;
}

void log_port_init(logFunc_t* dev)
{
    //此处将NULL修改为硬件接口结构体地址，例如：&huart1
    dev->hardware = NULL;
    //此处绑定串口初始化函数
    dev->log_hardware_init = NULL;
    //此处绑定串口deinit函数
    dev->log_hardware_deinit = NULL;
    dev->transmitFunc = stm32_usart_transmit_wrapping;
    dev->receiveFunc = stm32_usart_receive_wrapping;
}
