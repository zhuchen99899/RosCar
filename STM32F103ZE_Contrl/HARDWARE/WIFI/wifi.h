#ifndef __wifi_H_
#define __wifi_H_
#include "sys.h"
#include "stdio.h"	
#include "string.h"
#include "config.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "stdio.h"

#include "semphr.h"

void usart2_init(u32 bound);
void DMA_USART2_Init(void);
void NVIC_cofig_Init(void);
void wifi_init(u32 bound);
void USART2_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void WIFI_send(u8 * buffer,u32 len);

#endif

