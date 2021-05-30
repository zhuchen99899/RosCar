#ifndef __Task_IDLETASK_H
#define __Task_IDLETASK_H
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "led.h"
#include "OS_AppObjCreate.h"
#include "Car_config.h"


void Idle_task(void *pvParameters);
#endif

