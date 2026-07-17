#include "bsp_serial_printf.hpp"

using namespace bsp;

serial_printf::serial_printf(TramsmintCallback sendCallback, ReceiveCallback receiveCallback) : _sendCallback(sendCallback), _receiveCallback(receiveCallback), txFlag(0), rxFlag(0), txLen(0), rxLen(0) {
    // Initialize the txBuff and rxBuff to zero
    memset(txBuff, 0, sizeof(txBuff));
    memset(rxBuff, 0, sizeof(rxBuff));
}

const uint8_t* serial_printf::getTxBuff(void) const {
    return txBuff;
}

uint8_t* serial_printf::getRxBuff(void) {
    return rxBuff;
}

const uint8_t* serial_printf::getRxBuff(void) const {
    return rxBuff;
}

void serial_printf::setTxFlag(uint8_t flag) {
    txFlag = flag;
}

uint8_t serial_printf::getTxFlag(void) {
    return txFlag;
}

void serial_printf::setRxFlag(uint8_t flag) {
    rxFlag = flag;
}

uint8_t serial_printf::getRxFlag(void) {
    return rxFlag;
}

void serial_printf::setTxLen(uint16_t len) {
    txLen = len;
}

uint16_t serial_printf::getTxLen(void) {
    return txLen;
}

void serial_printf::setRxLen(uint16_t len) {
    rxLen = len;
}

uint16_t serial_printf::getRxLen(void) {
    return rxLen;
}

void serial_printf::printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    txLen = vsnprintf(reinterpret_cast<char*>(txBuff), sizeof(txBuff), format, args);
    va_end(args);

    if (nullptr != _sendCallback && txLen > 0) {
        _sendCallback(txBuff, txLen);
    }
}

void serial_printf::scanf(void) {
    if (nullptr != _receiveCallback) {
        _receiveCallback(rxBuff, sizeof(rxBuff));
    }
}
