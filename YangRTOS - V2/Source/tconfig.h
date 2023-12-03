#ifndef __TCONFIG_H

#define __TCONFIG_H



//任务优先级个数
#define TINYOS_PRO_COUNT 					32


//任务状态
#define TINYOS_TASK_STATE_RDY 				0 
#define TINYOS_TASK_STATE_DELAYED 			(1<<1)


//时间片初始值(单位：时间片周期10ms)
#define TINYOS_SLICE_MAX                    10




#endif