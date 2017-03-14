################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../StdPeriph/newlib/_exit.c \
../StdPeriph/newlib/_syscalls.c \
../StdPeriph/newlib/assert.c 

CPP_SRCS += \
../StdPeriph/newlib/_cxx.cpp 

OBJS += \
./StdPeriph/newlib/_cxx.o \
./StdPeriph/newlib/_exit.o \
./StdPeriph/newlib/_syscalls.o \
./StdPeriph/newlib/assert.o 

C_DEPS += \
./StdPeriph/newlib/_exit.d \
./StdPeriph/newlib/_syscalls.d \
./StdPeriph/newlib/assert.d 

CPP_DEPS += \
./StdPeriph/newlib/_cxx.d 


# Each subdirectory must supply rules for building sources it contributes
StdPeriph/newlib/%.o: ../StdPeriph/newlib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -ffunction-sections -fdata-sections  -g3 -DSTM32F10X_XL -DDEBUG -DHSE_VALUE=8000000 -DF_CPU=72000000 -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

StdPeriph/newlib/%.o: ../StdPeriph/newlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -ffunction-sections -fdata-sections  -g3 -DSTM32F10X_XL -DDEBUG -DHSE_VALUE=8000000 -DF_CPU=72000000 -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


