/*
 * Kernel.h
 *
 *  Created on: 31 марта 2017 г.
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
			char* TaskName;				 // имя задачи
			unsigned char name_length;	 // длинна имени задачи
			unsigned int *stk_top; 		 // вершина стека (последний сохраненный в стек элемент)
			unsigned int *stk_start; 	 // начало памяти, зарезервированной под стек
			unsigned int stk_size; 		 // размер стека задачи
			unsigned char task_priority; // приоритет задачи
			unsigned char task_state;    // текущее состояние задачи
			unsigned char taskID;	     // идентификатор задачи
			bool firstStart;		     // флаг первого старта задачи
			char deletedTask;            // флаг удаленной задачи
			void (*task_func)(void);     // указатель на функцию задачи
			unsigned int  _wait;         // задержка типа слип
			unsigned long _stamp;		 //
			unsigned char data_stack[800];   // указатель на стек
			unsigned char* heap;		 // указатель на кучу процесса
			void* pointerobj;		     // указатель на объект если это класс наследник Thread или делегат
			bool Is_delegate;			 // флаг
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
