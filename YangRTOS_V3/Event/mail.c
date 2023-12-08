#include "mail.h"





void tMboxInit(tMbox* mbox,void** msgBuffer,uint32_t maxCount){
	
	tEventInit(&(mbox->event),tEventTypeMbox);
	mbox->msgBuffer = msgBuffer;
	mbox->read = 0;
	mbox->write = 0;
	mbox->count = 0;
	mbox->maxCount = maxCount;
	
}


uint32_t tMboxWait(tMbox* mbox,void **msg,uint32_t waitTicks){
	uint32_t temp;
	uint32_t status = tTaskEnterCritical();
	if(mbox->count>0){
		mbox->count--;
		temp = (uint32_t)mbox->msgBuffer[mbox->read];
		*msg = mbox->msgBuffer[mbox->read++];
		if(mbox->read > mbox->maxCount){
			mbox->read = 0;
		}
		tTaskExitCritical(status);
		return tErrorNoError;
	}
	else{
		tEventWait(&(mbox->event),currentTask,(void*)0,0,waitTicks);
		tTaskExitCritical(status);
		tTaskSched();
		*msg = currentTask->eventMsg;
		return currentTask->waitEventResult;
		
		
	}
		

}

uint32_t tMboxNoWaitGet(tMbox* mbox,void **msg){
	uint32_t status = tTaskEnterCritical();
	if(mbox->count>0){
		mbox->count--;
		*msg = mbox->msgBuffer[mbox->read++];
		if(mbox->read > mbox->maxCount){
			mbox->read = 0;
		}
		tTaskExitCritical(status);
		return tErrorNoError;
	}
	else{
		tTaskExitCritical(status);
		return tErrorResourceUnavailable;
	}
}


//
uint32_t tMboxNotify(tMbox* mbox,void* msg){
	uint32_t status = tTaskEnterCritical();
	if(tEventWaitCount(&(mbox->event)) > 0){
		tTask* task = tEventWake(&(mbox->event),(void*)msg,tErrorNoError);
		if(task->priority < currentTask->priority){
			tTaskExitCritical(status);
			tTaskSched();
		}
	}
	else{
		if(mbox->count>=mbox->maxCount){
			tTaskExitCritical(status);
			return tErrorResourceFull;
		}
		else{
			mbox->msgBuffer[mbox->write++] = msg;
			if(mbox->write>mbox->maxCount){
				mbox->write = 0;
			}
			mbox->count++;
		}
	
	}
	tTaskExitCritical(status);
	return tErrorNoError;
	
}

void tMboxFlush(tMbox* mbox){
	uint32_t status= tTaskEnterCritical();
	if(tEventWaitCount(&(mbox->event))==0){
		mbox->read = 0;
		mbox->write = 0;
		mbox->count = 0;
	}
	
	tTaskExitCritical(status);
}



uint32_t tMboxDestroy(tMbox* mbox){
	uint32_t status= tTaskEnterCritical();
	uint32_t count = tEventRemoveAll(&(mbox->event),(void*)0,tErrorDel);
	tTaskExitCritical(status);
	if(count > 0){
		tTaskSched();
	
	}
	
	return count;
}













