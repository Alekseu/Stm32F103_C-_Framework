#ifndef OS_PORT_H_
#define OS_PORT_H_

#include "../StdPeriph/cmsis_boot/stm32f10x.h"
#include "kernel.h"
using namespace Os;

#define portNVIC_SYSPRI2_REG				( * ( ( volatile unsigned long * ) 0xe000ed20 ) )
#define portNVIC_PENDSV_PRI					( ( ( unsigned long ) 255 ) << 16UL )
#define portNVIC_SYSTICK_PRI				( ( ( unsigned long ) 255 ) << 24UL )

#define __SVC()     asm volatile ("SVC 0")
#define __PENDSV()  (SCB->ICSR = ( 1UL << 28UL ) )

#define BASEPRI_SET    (PRIO_SVCALL << (8 - __NVIC_PRIO_BITS))
#define BASEPRI_UNSET  (0x0)

#define SCHEDULER_DISABLE() asm volatile \
    ("mov r0, #0xe0     \n"\
     "MSR basepri, r0   \n" : : "i" (BASEPRI_SET) : "memory", "r0")

#define SCHEDULER_ENABLE()  asm volatile \
    ("mov r0, #0x00     \n"\
     "MSR basepri, r0   \n" : : "i" (BASEPRI_UNSET) : "memory", "r0")

#define enterCritical()  asm volatile ("cpsid   i" : : : "memory")
#define exitCritical()  asm volatile ("cpsie   i" : : : "memory")

#define enable_interrupts() __asm__ __volatile__ ("cpsie i")
#define disable_interrupts() __asm__ __volatile__ ("cpsid i")


#define __SVC()     asm volatile ("SVC 0")
#define IRQ_RETURN_PSP 0xFFFFFFFD
#define IRQ_RETURN_MSP 0xFFFFFFF9
#define IRQ_RETURN_ISR 0xFFFFFFF1

#define portNVIC_INT_CTRL_REG		( * ( ( volatile unsigned long * ) 0xe000ed04 ) )
#define portNVIC_PENDSVSET_BIT		( 1UL << 28UL )
#define portNVIC_PENDSVCLEAR_BIT 			( 1UL << 27UL )

#define PRIO_LEVELS     (1 << __NVIC_PRIO_BITS)
#define PRIO_MINIMUM    (PRIO_LEVELS - 1)
#define PRIO_MAXIMUM    (0)
#define PRIO_PENDSV     (PRIO_MINIMUM)
#define PRIO_SYSTICK    (PRIO_MINIMUM - 1)
#define PRIO_SVCALL     (PRIO_MINIMUM)



extern "C"
{
	extern uint32_t SystemCoreClock;
	//номер текущей задачи
	volatile unsigned int currentLoadTask;
	// номер загружаемой задачи
	volatile unsigned int TaskToLoad;

	volatile uint32_t _sp;

	Kernel::Task TaskList[TASK_LIST_POOL_COUNT];

	void __attribute__((naked))	SysTick_Handler(void)
	{
		if(Kernel::KernelObj->KernelStarted)
				{
					Kernel::KernelObj->ThreadTicks++;
					Kernel::KernelObj->SwitchContext();
				}
	}

	void __attribute__((naked))	 PendSV_Handler(void)
	{
		asm volatile (
				"MRS    r0, psp         \n" // get the user stack pointer
				"STMDB  r0!, {r4-r11}   \n" // push r4-r11 on the user stack and dec r0
				"MSR    psp, r0         \n" // update stack pointer
				: : :
				"r0", "r4", "r5", "r6", "r8", "r9", "r10", "r11", "memory");

		SCHEDULER_DISABLE();

		_sp = (unsigned int)&(TaskList[currentLoadTask].sp_);
		// Now save the stack pointer to THREAD(ACTIVE)->sp_
		asm volatile (
				"MRS    r0, psp     \n" // get the user stack pointer
				"STR    r0, [%0]    \n" // save new sp value
				: :
				"r" (_sp) :
				"r0", "memory");

		currentLoadTask =TaskToLoad;// SimpleKernel::KernelPointer->_currenttask;

		_sp = (unsigned int)&(TaskList[TaskToLoad].sp_ );

		asm volatile (
				"LDR    r0, [%0]    \n" // load new sp for new thread
				"MSR    psp, r0     \n" // set user stack pointer
				: :
				"r" (_sp) :
				"r0", "memory");


		SCHEDULER_ENABLE();

		// Restore the stack and return to PSP (PendSV is lowest priority,
		// so it can never preempt another interrupt or itself, only user code)
		asm volatile (
				"MRS    r0, psp             \n" // load new sp for new thread
				"LDMIA  r0!, {r4-r11}       \n" // pop r4-r11 from stack, inc r0
				"    TST       LR, #0x10         \n" // exc_return[4]=0? (it means that new process
				"MSR    psp, r0             \n" // adjust user stack pointer
				"BX     lr                  \n" // return
				: : :
				"r0", "r4", "r5", "r6", "r8", "r9", "r10", "r11", "memory"

		);
}

	void __attribute__((naked)) SVC_Handler(void)
		{
			asm volatile (
					"MOV    r0, lr  \n" // If SVC gets called from the main stack
					"CMP    r0, %0  \n" // return to the user stack.
					"ITT    eq      \n"
					"MOVEQ  r0, %1  \n"
					"BXEQ   r0      \n"
					: :
					"i" (IRQ_RETURN_MSP), "i" (IRQ_RETURN_PSP) :
					"r0", "cc");

			// Since PendSV is naked, lr is wrong now, so load the user return value
			asm volatile (
					"BL     PendSV_Handler  \n"
					"MOV    lr, %0          \n"
					"bx     lr              \n"
					: :
					"i" (IRQ_RETURN_PSP) :);
		}



//	void SwitchContextHandler(void)
//	{
//
////		if(currentLoadTask==255)
////		{
////			currentLoadTask =0;
////		}
////
////		//if(currentLoadTask ==TaskToLoad )  return;
////
//		asm volatile (
//				"MRS    r0, psp         \n" // get the user stack pointer
//				"STMDB  r0!, {r4-r11}   \n" // push r4-r11 on the user stack and dec r0
//				"MSR    psp, r0         \n" // update stack pointer
//				: : :
//				"r0", "r4", "r5", "r6", "r8", "r9", "r10", "r11", "memory");
//
//		SCHEDULER_DISABLE();
//
//		_sp = (uint32_t)&(Kernel::KernelObj->TaskList[currentLoadTask].sp_);
//		// Now save the stack pointer to THREAD(ACTIVE)->sp_
//		asm volatile (
//				"MRS    r0, psp     \n" // get the user stack pointer
//				"STR    r0, [%0]    \n" // save new sp value
//				: :
//				"r" (_sp) :
//				"r0", "memory");
//
//		currentLoadTask =TaskToLoad;// SimpleKernel::KernelPointer->_currenttask;
//
//		_sp = (uint32_t)&(Kernel::KernelObj->TaskList[TaskToLoad].sp_ );
//
//		asm volatile (
//				"LDR    r0, [%0]    \n" // load new sp for new thread
//				"MSR    psp, r0     \n" // set user stack pointer
//				: :
//				"r" (_sp) :
//				"r0", "memory");
//
//
//		SCHEDULER_ENABLE();
//
//		// Restore the stack and return to PSP (PendSV is lowest priority,
//		// so it can never preempt another interrupt or itself, only user code)
//		asm volatile (
//				"MRS    r0, psp             \n" // load new sp for new thread
//				"LDMIA  r0!, {r4-r11}       \n" // pop r4-r11 from stack, inc r0
//				"    TST       LR, #0x10         \n" // exc_return[4]=0? (it means that new process
//				"MSR    psp, r0             \n" // adjust user stack pointer
//				"BX     lr                  \n" // return
//				: : :
//				"r0", "r4", "r5", "r6", "r8", "r9", "r10", "r11", "memory"
//
//		);
//}
//
//	void SwcHandler(void)
//		{
//			asm volatile (
//					"MOV    r0, lr  \n" // If SVC gets called from the main stack
//					"CMP    r0, %0  \n" // return to the user stack.
//					"ITT    eq      \n"
//					"MOVEQ  r0, %1  \n"
//					"BXEQ   r0      \n"
//					: :
//					"i" (IRQ_RETURN_MSP), "i" (IRQ_RETURN_PSP) :
//					"r0", "cc");
//
//			// Since PendSV is naked, lr is wrong now, so load the user return value
//			asm volatile (
//					"BL     PendSV_Handler  \n"
//					"MOV    lr, %0          \n"
//					"bx     lr              \n"
//					: :
//					"i" (IRQ_RETURN_PSP) :);
//		}

	int SetupSysTick(int ticks)
			{
				while (SysTick_Config(SystemCoreClock / 1000) != 0) {
				} // One SysTick interrupt now equals 1us
				return (0);
			}
	void SetupNvicPriority()
	{
		portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
		portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;
	}


	void _PendSV()
				{
					__PENDSV();
				}

				void _Svc()
				{
					__SVC();
				}

}
#endif /* OS_PORT_H_ */
