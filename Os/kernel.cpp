/*
 * kernel.cpp
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */
#include "kernel.h"
#include "port.h"
#include "../Driver/nvic/nvic.h"

namespace Os
{
	using namespace Driver;

	Kernel* Kernel::KernelObj=0;
	//unsigned long ThreadTicks;
	static uint32_t _sp;


	static void schedExit(void* return_value)
	{
		TaskList[currentLoadTask].deletedTask=0;
		delete[] TaskList[currentLoadTask].data_stack;
		delete[] TaskList[currentLoadTask].heap;
		Kernel::KernelObj->SwitchContext();
	}

	static void IdleTask()
	{
		static long LastTick;
		static unsigned int count;             //наш труд¤га счетчик
		static unsigned int max_count ;

		while(1)
		{
			//максимальное значение счетчика, вычисл¤етс¤ при калибровке и соответствует 100% CPU idle
			count++;  //приращение счетчика

			if((Kernel::KernelObj->ThreadTicks-LastTick > 10)|| (LastTick-Kernel::KernelObj->ThreadTicks>0 && LastTick-Kernel::KernelObj->ThreadTicks>10))
			{ //если прошло 1000 тиков (1 сек дл¤ моей платфрмы)
				LastTick = Kernel::KernelObj->ThreadTicks;
				if(count > max_count) max_count = count;          //это калибровка
				Kernel::KernelObj->CpuLoad =  100-(100 * count / max_count);         //вычисл¤ем текущую загрузку
				count = 0;     //обнул¤ем счетчик

			}



		}
	}

	void Kernel::OnTick()
	{
		if(Kernel::KernelObj->KernelStarted)
		{
			Kernel::KernelObj->ThreadTicks++;
			Kernel::KernelObj->SwitchContext();
		}
	}

	Kernel::Kernel()
	{
		Kernel::KernelObj = this;
		CpuLoad =0;
		first_start =true;
		_initTaskSlots();
		_tasklength=0;

		Task idle;
		idle.TaskName = (char*)"IDLE";
		idle.name_length = 4;
		idle.stk_size = 256;
		//idle.data_stack = new  unsigned char[idle.stk_size]; // todo playsment new
		memset(idle.data_stack , 0 ,idle.stk_size);
		idle.stk_start = (unsigned int*)(&idle.data_stack[idle.stk_size-1]);
		idle.stk_top = (unsigned int*)(&idle.data_stack[idle.stk_size-1]);
		idle.task_func = IdleTask;
		idle.task_priority = 4;
		idle.heap = new unsigned char[idle.stk_size];

		InitStack *stack_frame = (InitStack*)((uint32_t)idle.data_stack + (uint32_t)idle.stk_size - (uint32_t)sizeof(InitStack));
		stack_frame->psr =  0x01000000UL ;
		stack_frame->pc = idle.task_func;
		// Reset value for return, shoudln't get reached
		stack_frame->lr = (uint32_t)&schedExit;

		// Save stack pointer
		idle.sp_ = stack_frame;
		__set_PSP((uint32_t)idle.sp_ + 8 * 4);

		idle._wait = 0;
		idle.deletedTask =1;

		TaskList[_tasklength++] = idle;
		currentLoadTask =0;

	}

	Kernel::~Kernel()
	{

	}

	void Kernel::Init()
	{
//		InterruptController::EnableChannel(PendSV_IRQn);
//		InterruptController::SetHandler(PendSV_IRQn,SwitchContextHandler);
//
//		InterruptController::EnableChannel(SVCall_IRQn);
//		InterruptController::SetHandler(SVCall_IRQn,SwcHandler);
		SetupNvicPriority();
		__disable_irq();
		SetupSysTick(1000);


	}

	void Kernel::StartScheduler()
	{
		KernelStarted = true;
		__enable_irq();
		_Svc();
	}

	void Kernel::SwitchContext()
	{
		// set idele task by deafult

//		if(_isTaskElapsed(currentLoadTask))
//		{
//			TaskToLoad =currentLoadTask;
//		}
//		else
//		{
			TaskToLoad=0;
//		}

//		// пока что просто пробегаем по всем слотам дл¤ задач провер¤ем какую необходимо вызвать (без приоритетов)
//		for(int i=1;i<TASK_LIST_POOL_COUNT-1;i++)
//		{
//			bool task1_elapsed =_isTaskElapsed(i);
//			bool task2_elapsed = _isTaskElapsed(i+1);
//			//if(_taskList[i].deletedTask&&_taskList[i]._wait->IsElapsed() )
//			if((task1_elapsed && TaskList[i].deletedTask) &&  (TaskList[i].task_priority<TaskList[i+1].task_priority) && (( task2_elapsed && TaskList[i+1].deletedTask)||( task2_elapsed && !TaskList[i+1].deletedTask)) )
//			{
//				TaskToLoad = i;
//				break;
//			}
//		}

		//если не получилось найти задачу на запуск по времени и приоритетам, то запускаем любую котора¤ готова к выполнению
		if(TaskToLoad==currentLoadTask)
		{
			for(int i=1;i<TASK_LIST_POOL_COUNT;i++)
			{

				if(TaskList[i].deletedTask && _isTaskElapsed(i))
				{
					TaskToLoad = i;
					break;
				}
			}
		}
//		if(currentLoadTask == TaskToLoad) return;
//
//		if(TaskToLoad==0) TaskList[TaskToLoad]._stamp = ThreadTicks;

		TaskList[currentLoadTask].task_state = Kernel::Suspended;
		TaskList[TaskToLoad].task_state = Kernel::Running;


		// в противном случае будет запущен IDLE
		_PendSV();
	}

	int Kernel::AddTask(void (*method)(void), int stacksize ,  unsigned char priority, char* taskname, void* obj, bool delegate )
	{
		EnterCritical();
		Task task;
		if(stacksize!=0)
		{
			task.stk_size = stacksize;
		}
		else
		{
			task.stk_size = 256;
		}

		task.TaskName = taskname;
		task.name_length = strlen(taskname);
		//task.data_stack = new unsigned char[task.stk_size];//new (kernelHeap) unsigned char[task.stk_size]; //размещающий new
		memset(task.data_stack , 0 ,task.stk_size);
		task.stk_start = (unsigned int*)(&task.data_stack[task.stk_size-1]);
		task.stk_top = (unsigned int*)(&task.data_stack[task.stk_size-1]);
		task.heap = new unsigned char[task.stk_size];//new (kernelHeap) unsigned char[task.stk_size];

		task.task_func = method;
		task.task_priority = priority;

		InitStack *stack_frame = (InitStack*)((uint32_t)task.data_stack + (uint32_t)task.stk_size - (uint32_t)sizeof(InitStack));

		stack_frame->psr =  0x01000000UL ;
		stack_frame->pc = task.task_func;
		// Reset value for return, shoudln't get reached
		stack_frame->lr = (uint32_t)&schedExit;

		task._wait =0;
		// Save stack pointer
		task.sp_ = stack_frame;

		task.pointerobj = obj;


		task.taskID = _tasklength;
		task.Is_delegate = delegate;
		task.firstStart = obj==0? true:false;

		task.deletedTask =1;
		int taskid=-1;
		//ищем свободный слот
		for(int i=1;i<TASK_LIST_POOL_COUNT;i++)
		{
			if(TaskList[i].deletedTask==0)
			{
				TaskList[i] = task;
				taskid=i;
				break;
			}
		}

		ExitCritical();
		return taskid;
	}

	void Kernel::DeleteTask(char taskId)
	{
		EnterCritical();
		TaskList[taskId].deletedTask =0;
		_tasklength--;

		// освобождаем пам¤ть
		delete[] TaskList[taskId].data_stack;
		delete[] TaskList[taskId].heap;
		// тут необходимо освободить пам¤ть перенастройть все указатели если используем размещающий new

		SwitchContext();
		ExitCritical();
	}

	void Kernel::Delay(unsigned int del)
	{
		EnterCritical();
		TaskList[currentLoadTask]._wait=del;
		TaskList[currentLoadTask]._stamp=ThreadTicks;
		ExitCritical();
		SwitchContext();
	}

	void Kernel::Yeld()
	{
		TaskList[currentLoadTask]._wait=1;
		TaskList[currentLoadTask]._stamp=ThreadTicks;
		SwitchContext();
	}

	void Kernel::EnterCritical()
	{
		enterCritical();
	}

	void Kernel:: ExitCritical()
	{
		exitCritical();
	}

	void  Kernel::_initTaskSlots()
	{
		for(int i=0;i<TASK_LIST_POOL_COUNT;i++)
		{
			TaskList[i].deletedTask=0;
			//TaskList[i].data_stack=0;
			TaskList[i]._wait=0;
			TaskList[i]._stamp=0;
			TaskList[i].heap=0;
			TaskList[i].firstStart = true;
			TaskList[i].pointerobj=0;
			TaskList[i].stk_size=0;
			TaskList[i].stk_start=0;
			TaskList[i].stk_top=0;
			TaskList[i].taskID=0;
			TaskList[i].task_func=0;
			TaskList[i].task_priority=255;
			TaskList[i].task_state = 1;
			TaskList[i].Is_delegate=false;
		}
	}

	bool Kernel::_isTaskElapsed(unsigned char currentTask)
	{
		if (TaskList[currentTask]._stamp > ThreadTicks) return true;

		if (TaskList[currentTask]._stamp + TaskList[currentTask]._wait < ThreadTicks) return true;

		return false;
	}



}



