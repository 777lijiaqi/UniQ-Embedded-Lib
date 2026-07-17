#ifndef BSP_SERIAL_PRINTF_HPP
#define BSP_SERIAL_PRINTF_HPP

#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <cstdio>

namespace bsp{
    class serial_printf{
        using TramsmintCallback = void(*)(uint8_t* buff , uint16_t len);
        using ReceiveCallback = void(*)(uint8_t* buff , uint16_t len);
        private:
            TramsmintCallback _sendCallback;
            ReceiveCallback _receiveCallback;
            uint8_t txBuff[1024];
            uint8_t rxBuff[1024];
            uint8_t txFlag; 
            uint8_t rxFlag;
            uint16_t txLen;
            uint16_t rxLen;
        public:
            serial_printf(TramsmintCallback sendCallback = nullptr, ReceiveCallback receiveCallback = nullptr);
            const uint8_t* getTxBuff(void) const;
            uint8_t* getRxBuff(void);
            const uint8_t* getRxBuff(void) const;
            void setTxFlag(uint8_t flag);
            uint8_t getTxFlag(void);
            void setRxFlag(uint8_t flag);
            uint8_t getRxFlag(void);
            void setTxLen(uint16_t len);
            uint16_t getTxLen(void);
            void setRxLen(uint16_t len);
            uint16_t getRxLen(void);  
            void printf(const char *format, ...);
            void scanf(void);
    };
}


#endif
