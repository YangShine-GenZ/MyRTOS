#include "time.h"





void tSetSysTickPeriod(uint32_t ms){
	SysTick->LOAD = ms*SystemCoreClock/1000-1; //ms
	NVIC_SetPriority(SysTick_IRQn,(1<<__NVIC_PRIO_BITS)-1);
	SysTick->VAL  = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

}




void tTaskDelay(uint32_t delay){
	currentTask->delayTicks = delay;
	tTaskSched();


}




void tTaskSystemTickHandler(void){
	int i;
	for(i = 0;i<2;i++){
		if(taskTable[i]->delayTicks > 0){
			taskTable[i]->delayTicks --;
		}
		
	}
	tTaskSched();

}




void SysTick_Handler(){
	tTaskSystemTickHandler();

}