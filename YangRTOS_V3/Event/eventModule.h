#ifndef __EVENTMODULE_H
#define __EVENTMODULE_H


#include "tlib.h"


//´íÎóÂë
typedef enum _tError{
	tErrorNoError = 0,
	tErrorTimeOut,
	tErrorResourceUnavailable,
	tErrorResourceFull,
	tErrorDel,
	tErrorOwner,
}tError;



typedef enum _tEventType{
	tEventTypeUnknown  = 0,
	tEventTypeSem = 1,
	tEventTypeMbox = 2,
	tEventTypeMemBlock = 3,
	tEventTypeFlagGroup = 4,
	tEventTypeMutex = 5,
}tEventType;



typedef struct _tEvent{

	tEventType type;
	//µÈ´ý¶ÓÁÐ
	tList waitlist;

}tEvent;


#define TFLAGGROUP_CLEAR     (0x0 << 0)
#define TFLAGGROUP_SET       (0x1 << 0)
#define TFLAGGROUP_ANY		 (0x0 << 1)
#define TFLAGGROUP_ALL		 (0x1 << 1)

#define TFLAGGROUP_SET_ALL     (TFLAGGROUP_SET | TFLAGGROUP_ALL)
#define TFLAGGROUP_SET_ANY     (TFLAGGROUP_SET | TFLAGGROUP_ANY)
#define TFLAGGROUP_CLEAR_ALL   (TFLAGGROUP_CLEAR | TFLAGGROUP_ALL)
#define TFLAGGROUP_CLEAR_ANY   (TFLAGGROUP_CLEAR | TFLAGGROUP_ANY)

#define TFLAGGROUP_CONSUME   (1<<7)



#endif