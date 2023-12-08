#include "primask.h"



uint32_t tTaskEnterCritical(){
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	return primask;
}





void tTaskExitCritical(uint32_t val){

	__set_PRIMASK(val);

}