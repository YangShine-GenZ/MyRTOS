#ifndef __PRIMASK_H



#define __PRIMASK_H



#include <stdint.h>
#include "ARMCM3.h"


uint32_t tTaskEnterCritical();
void tTaskExitCritical(uint32_t val);



#endif 
