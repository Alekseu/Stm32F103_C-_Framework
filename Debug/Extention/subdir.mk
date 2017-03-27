################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Extention/MemoryManager.cpp \
../Extention/delay.cpp \
../Extention/operators.cpp 

OBJS += \
./Extention/MemoryManager.o \
./Extention/delay.o \
./Extention/operators.o 

CPP_DEPS += \
./Extention/MemoryManager.d \
./Extention/delay.d \
./Extention/operators.d 


# Each subdirectory must supply rules for building sources it contributes
Extention/%.o: ../Extention/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -ffunction-sections -fdata-sections  -g3 -DSTM32F10X_XL -DDEBUG -DHSE_VALUE=8000000 -DF_CPU=72000000 -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


