#include "tflag.h"





void tFlagGroupInit(tFlagGroup* flagGroup,uint32_t flags){
	tEventInit(&flagGroup->event,tEventTypeFlagGroup);
	flagGroup->flags = flags;

}



static uint32_t tFlagGroupCheckConsume(tFlagGroup* flagGroup,uint32_t type,uint32_t* flags){
	uint32_t srcFlag = *flags;
	uint32_t isSet = type&TFLAGGROUP_SET;
	uint32_t isAll = type&TFLAGGROUP_ALL;
	uint32_t isConsume = type&TFLAGGROUP_CONSUME;
	
	uint32_t calFlag = isSet? (flagGroup->flags & srcFlag):(~flagGroup->flags & srcFlag);
	
	if(((isAll!=0)&&(calFlag==srcFlag)) || ((isAll == 0)&&(calFlag!=0))){
		if(isConsume){
			if(isSet){
				flagGroup->flags &= ~srcFlag;
			
			}
			else{
				flagGroup->flags |= srcFlag;
			}
		}
		*flags = calFlag;
		return tErrorNoError;
	
	}
	*flags = calFlag;
	return tErrorResourceUnavailable;
}







uint32_t tFlagGroupWait(tFlagGroup* flagGroup,uint32_t waitType, uint32_t requestFlag,uint32_t *resultFlag,uint32_t waitTicks){}



uint32_t tFlagGroupNoWaitGet(tFlagGroup* flagGroup,uint32_t waitType, uint32_t requestFlag,uint32_t *resultFlag){}
	
	

void tFlagGroupNotify(tFlagGroup* flagGroup,uint8_t isSet,uint32_t flag){}