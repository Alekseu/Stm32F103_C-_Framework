################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../StdPeriph/cmsis_boot/startup.c 

OBJS += \
./StdPeriph/cmsis_boot/startup.o 

C_DEPS += \
./StdPeriph/cmsis_boot/startup.d 


# Each subdirectory must supply rules for building sources it contributes
StdPeriph/cmsis_boot/%.o: ../StdPeriph/cmsis_boot/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -ffunction-sections -fdata-sections  -g3 -DSTM32F10X_HD -DDEBUG -DHSE_VALUE=8000000 -DF_CPU=72000000 -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


