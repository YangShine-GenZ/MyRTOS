#include "tinyOS.h"
#include "time.h"


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


int ShareCount = 0;


void task1DestroyFun(void* params){
	flag1 = 0;
}


void task1(void* parms){
	
	tSetSysTickPeriod(10);
	
	tTaskSetCleanFunc(currentTask,task1DestroyFun,(void*)0);
	for(;;)
	{	
		flag1 = 0;
		tTaskDelay(1);	
		flag1 = 1;
		tTaskDelay(1);
	}
}


void task2(void* parms){
	
	int task1Deleted = 0;
	
	for(;;){
		
		flag2 = 0;
		tTaskDelay(1);
		flag2 = 1;
		tTaskDelay(1);
		
		if(task1Deleted == 0){
			tTaskForceDelete(&tTask1);
			task1Deleted = 1;
		}
		
		
	}
}

void task3(void* parms){
	for(;;){
		
		if(tTaskGetDelete()){
			flag3 = 0;
			tTaskDeleteSelf();
		}
		
		flag3 = 0;
		tTaskDelay(1);
		flag3 = 1;
		tTaskDelay(1);
		
	}
}

void task4(void* parms){
	
	int task3Deleted = 0;
	for(;;){
		
		flag4 = 0;
		tTaskDelay(1);
		flag4 = 1;
		tTaskDelay(1);
		
		if(!task3Deleted){
			tTaskRequestDelete(&tTask3);
			task3Deleted = 1;
		}
		
	
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
	tTaskInit(&tTask3,task3,(void*)0x22222222,&task3Env[1024],0);
	tTaskInit(&tTask4,task4,(void*)0x33333333,&task4Env[1024],1);
	
	tTaskInit(&tTaskIdle,taskIdle,(void*)0x0,&taskIdleEnv[1024],TINYOS_PRO_COUNT-1);
	IdleTask = &tTaskIdle;
	

	
	nextTask = tTaskHighestReady();


	
	//run first task
	tTaskRunFirst();
	
	
	return 0;
}

