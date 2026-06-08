#include "unqi_log.h"

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
