#ifndef CRC16_H
#define CRC16_H
#include "stdint.h"

uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen);
unsigned long Combine_CRC_2Byte(unsigned char high_byte,unsigned char low_byte);

#endif // CRC16_H
