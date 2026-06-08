#ifndef __UNIQ_LOG_H
#define __UNIQ_LOG_H

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#define LOG_TX_BUFF_SIZE    256
#define LOG_RX_BUFF_SIZE    256
#define LOG_TIME_OUT        0xFFFF

typedef enum{
    LOG_OK,
    LOG_TIMEOUT,
    LOG_ERROR,
    LOG_BUSY
}logState_t;

typedef logState_t (*log_transmit)(void* hardware , const uint8_t* pData , uint16_t size , uint32_t timOut);
typedef logState_t (*log_receive)(void* hardware , const uint8_t* pData , uint16_t size , uint32_t timOut);

typedef struct{
    void* hardware;
    void* log_hardware_init(void);
    log_transmit transmitFunc;
    log_receive receiveFunc;
    void* log_hardware_deinit(void);
}logFunc_t;

#endif
