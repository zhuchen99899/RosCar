#ifndef __CRC16_H
#define __CRC16_H

#include "stm32f10x.h"
#include "config.h"
u16 CRC16(uint8_t *_pBuf, int _usLen);
u16 Combine_CRC_2Byte(u8 high_byte,u8 low_byte);

#endif



