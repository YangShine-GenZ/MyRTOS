#ifndef __TCONFIG_H

#define __TCONFIG_H



//任务优先级个数
#define TINYOS_PRO_COUNT 					32


//任务状态
#define TINYOS_TASK_STATE_RDY 				0 
#define TINYOS_TASK_STATE_DESTORYER         (1<<1)
#define TINYOS_TASK_STATE_DELAYED 			(1<<2)
#define TINYOS_TASK_STATE_SUSPEND           (1<<3)


#define TYNYOS_TASK_WAIT_MASK               (0xFF << 16)



//时间片初始值(单位：时间片周期10ms)
#define TINYOS_SLICE_MAX                    10


//定时器参数
#define TINYOS_TIMERTASK_STACK_SIZE			1024
#define TINYOS_TIMERTASK_PRIO               1



#endif