#include "uniq_log.h"

uint16_t log_calculate_CRC16(uint8_t* data , uint16_t length)
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

logState_t log_printf_com(logFunc_t* dev , const char* format , ...)
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

logState_t log_receive_com(logFunc_t* dev , const uint8_t* pData , uint16_t size , uint32_t timOut)
{
    dev->receiveFunc(dev->hardware , pData , size , timOut);
}
