#ifndef _TIME_H
#define _TIME_H


#include "tinyOS.h"

void tSetSysTickPeriod(uint32_t ms);
void tTaskSystemTickHandler(void);
void tTaskDelay(uint32_t delay);



#endif