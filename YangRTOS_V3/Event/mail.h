#ifndef __MAIL_H
#define __MAIL_H



#include"tinyOS.h"
#include "event.h"



typedef struct _tMbox{
	
	tEvent event;
	uint32_t count;
	//�׵�ַ
	uint32_t read;
	//β��ַ
	uint32_t write;
	uint32_t maxCount;
	
	//��Ϣ��������ַ
	void** msgBuffer;

}tMbox;


void tMboxInit(tMbox* mbox,void** msgBuffer,uint32_t maxCount);
uint32_t tMboxWait(tMbox* mbox,void **msg,uint32_t waitTicks);
uint32_t tMboxNoWaitGet(tMbox* mbox,void **msg);
uint32_t tMboxNotify(tMbox* mbox,void* msg);
void tMboxFlush(tMbox* mbox);
uint32_t tMboxDestroy(tMbox* mbox);
	



#endif