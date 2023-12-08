#include "common.h"

void delay(){

	int i;
	for(i = 0;i<0xFF;i++);

}

int flag1;
int flag2;
int flag3;
int flag4;

tTask tTask1;
tTask tTask2;
tTask tTask3;
tTask tTask4;
tTask tTaskIdle;


tTaskStack task1Env[1024];
tTaskStack task2Env[1024];
tTaskStack task3Env[1024];
tTaskStack task4Env[1024];

tTaskStack taskIdleEnv[1024];


tTimer timer1;
tTimer timer2;
tTimer timer3;

uint32_t bit1 = 0;
uint32_t bit2 = 0;
uint32_t bit3 = 0;


void timerFunc(void* args){
	uint32_t *ptrBit = (uint32_t*)args;
	*ptrBit ^= 0x1;
}




//tMbox mbox1;
//tMbox mbox2;

//void* mbox1Buffer[20];
//void* mbox2Buffer[20];

//uint32_t msg[20];


int ShareCount = 0;

void DestroyFun(void* params){
	flag1 = 0;
}


void task1(void* parms){
	
	uint32_t stopped = 0;
	
	tSetSysTickPeriod(10);
	tTimerInit(&timer1,100,10,timerFunc,(void*)&bit1,TIMER_CONFIG_TYPE_HARD);
	tTimerStart(&timer1);
	tTimerInit(&timer2,200,20,timerFunc,(void*)&bit2,TIMER_CONFIG_TYPE_HARD);
	tTimerStart(&timer2);
	tTimerInit(&timer3,300,0,timerFunc,(void*)&bit3,TIMER_CONFIG_TYPE_SOFT);
	tTimerStart(&timer3);
	
	for(;;)
	{	
	
		flag1 = 0;
		tTaskDelay(1);	
		flag1 = 1;
		tTaskDelay(1);
		
		if(stopped == 0){
			tTaskDelay(200);
			tTimerStop(&timer1);
			stopped = 1;
		}
		
	}
}



void task2(void* parms){
	
	
	for(;;){
		flag2 = 0;
		tTaskDelay(1);
		flag2 = 1;
		tTaskDelay(1);
	}
		
}

void task3(void* parms){
	
	for(;;){
		flag3 = 0;
		tTaskDelay(1);
		flag3 = 1;
		tTaskDelay(1);
		
	}
}

void task4(void* parms){
	
	for(;;){
	
		flag4 = 0;
		tTaskDelay(1);
		flag4 = 1;
		tTaskDelay(1);
		
		
	
	}
}



void taskIdle(void* parms){
	for(;;)
	{
	}

}




int main(){
	
	tTaskSchedInit();
	tTimerModuleInit();
	tTaskInit(&tTask1,task1,(void*)0x11111111,&task1Env[1024],0);
	tTaskInit(&tTask2,task2,(void*)0x00000000,&task2Env[1024],1);
	tTaskInit(&tTask3,task3,(void*)0x22222222,&task3Env[1024],0);
	tTaskInit(&tTask4,task4,(void*)0x33333333,&task4Env[1024],1);
	
	tTaskInit(&tTaskIdle,taskIdle,(void*)0x0,&taskIdleEnv[1024],TINYOS_PRO_COUNT-1);
	IdleTask = &tTaskIdle;
	

	
	nextTask = tTaskHighestReady();


	
	//run first task
	tTaskRunFirst();
	
	
	return 0;
}

