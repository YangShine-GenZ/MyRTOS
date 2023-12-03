#include "tinyOS.h"
#include "time.h"



void delay(){

	int i;
	for(i = 0;i<0xFF;i++);

}

static int flag1;
static int flag2;
static int flag3;

tTask tTask1;
tTask tTask2;
tTask tTask3;
tTask tTaskIdle;


tTaskStack task1Env[1024];
tTaskStack task2Env[1024];
tTaskStack task3Env[1024];
tTaskStack taskIdleEnv[1024];


int ShareCount = 0;


void task1(void* parms){
	
	tSetSysTickPeriod(10);
	for(;;)
	{	
		flag1 = 0;
		tTaskDelay(1);	
		flag1 = 1;
		tTaskDelay(1);	
	}
}


void task2(void* parms){
	for(;;){
		flag2 = 0;
		delay();
		flag2 = 1;
		delay();
		
	}
}

void task3(void* parms){
	for(;;){
		flag3 = 0;
		delay();
		flag3 = 1;
		delay();
		
	}
}



void taskIdle(void* parms){
	for(;;)
	{
	}

}




int main(){
	
	tTaskSchedInit();
	tTaskInit(&tTask1,task1,(void*)0x11111111,&task1Env[1024],0);
	tTaskInit(&tTask2,task2,(void*)0x00000000,&task2Env[1024],1);
	tTaskInit(&tTask3,task3,(void*)0x22222222,&task3Env[1024],1);
	tTaskInit(&tTaskIdle,taskIdle,(void*)0x0,&taskIdleEnv[1024],TINYOS_PRO_COUNT-1);
	IdleTask = &tTaskIdle;
	

	
	nextTask = tTaskHighestReady();


	
	//run first task
	tTaskRunFirst();
	
	
	return 0;
}

