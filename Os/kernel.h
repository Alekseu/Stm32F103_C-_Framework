/*
 * Kernel.h
 *
 *  Created on: 31 ����� 2017 �.
 *      Author: hudienko_a
 */

#ifndef OS_KERNEL_H_
#define OS_KERNEL_H_

#include "../Driver/platform.h"
#include "OsConfig.h"

namespace Os
{

	class Kernel
	{
	public:
		typedef struct {
			uint32_t r4 __attribute__ ((packed));
			uint32_t r5 __attribute__ ((packed));
			uint32_t r6 __attribute__ ((packed));
			uint32_t r7 __attribute__ ((packed));
			uint32_t r8 __attribute__ ((packed));
			uint32_t r9 __attribute__ ((packed));
			uint32_t r10 __attribute__ ((packed));
			uint32_t r11 __attribute__ ((packed));
			uint32_t r0 __attribute__ ((packed));
			uint32_t r1 __attribute__ ((packed));
			uint32_t r2 __attribute__ ((packed));
			uint32_t r3 __attribute__ ((packed));
			uint32_t r12 __attribute__ ((packed));
			uint32_t lr __attribute__ ((packed));
			void (*pc)(void) __attribute__ ((packed));
			uint32_t psr __attribute__ ((packed));
		} InitStack;

		struct Task
		{
			char* TaskName;				 // ��� ������
			unsigned char name_length;	 // ������ ����� ������
			unsigned int *stk_top; 		 // ������� ����� (��������� ����������� � ���� �������)
			unsigned int *stk_start; 	 // ������ ������, ����������������� ��� ����
			unsigned int stk_size; 		 // ������ ����� ������
			unsigned char task_priority; // ��������� ������
			unsigned char task_state;    // ������� ��������� ������
			unsigned char taskID;	     // ������������� ������
			bool firstStart;		     // ���� ������� ������ ������
			char deletedTask;            // ���� ��������� ������
			void (*task_func)(void);     // ��������� �� ������� ������
			unsigned int  _wait;         // �������� ���� ����
			unsigned long _stamp;		 //
			unsigned char data_stack[800];   // ��������� �� ����
			unsigned char* heap;		 // ��������� �� ���� ��������
			void* pointerobj;		     // ��������� �� ������ ���� ��� ����� ��������� Thread ��� �������
			bool Is_delegate;			 // ����
			InitStack *sp_;
		};

		typedef enum _taskPriority
		{
			RealTime =0,
			Higest =1,
			Middle =2,
			Lowest =3,
			Idle =4
		} TaskPriority;

		typedef enum _taskstate
		{
			Running=0,
			Suspended=1
		}TaskState;

		public:
			static Kernel* KernelObj;
			unsigned long ThreadTicks;
			unsigned int CpuLoad;
			bool KernelStarted;


			Kernel();
			~Kernel();

			void Init();

			void StartScheduler();
			int AddTask(void (*method)(void), int stacksize, unsigned char priority, char* taskname, void* obj , bool delegate);
			void ResetTask(char taskId);
			void DeleteTask(char taskId);
			void Delay(unsigned int del);
			void Yeld();
			void EnterCritical();
			void ExitCritical();

			void SwitchContext();

			static void OnTick();
//			static void SwitchContextHandler();
//			static void Swc_Handler();

		private:
			bool first_start;
			unsigned char _tasklength;
			char kernelHeap[KERNEL_HEAP_SIZE];

			void _initTaskSlots();
			bool _isTaskElapsed(unsigned char currentTask);


	};
}


#endif /* OS_KERNEL_H_ */
