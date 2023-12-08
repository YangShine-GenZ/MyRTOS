#ifndef __TCONFIG_H

#define __TCONFIG_H



//�������ȼ�����
#define TINYOS_PRO_COUNT 					32


//����״̬
#define TINYOS_TASK_STATE_RDY 				0 
#define TINYOS_TASK_STATE_DESTORYER         (1<<1)
#define TINYOS_TASK_STATE_DELAYED 			(1<<2)
#define TINYOS_TASK_STATE_SUSPEND           (1<<3)


#define TYNYOS_TASK_WAIT_MASK               (0xFF << 16)



//ʱ��Ƭ��ʼֵ(��λ��ʱ��Ƭ����10ms)
#define TINYOS_SLICE_MAX                    10


//��ʱ������
#define TINYOS_TIMERTASK_STACK_SIZE			1024
#define TINYOS_TIMERTASK_PRIO               1



#endif