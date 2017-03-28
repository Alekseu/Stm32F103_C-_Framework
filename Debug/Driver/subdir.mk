################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Driver/Dma.cpp \
../Driver/Usb.cpp \
../Driver/timer.cpp \
../Driver/usart.cpp 

OBJS += \
./Driver/Dma.o \
./Driver/Usb.o \
./Driver/timer.o \
./Driver/usart.o 

CPP_DEPS += \
./Driver/Dma.d \
./Driver/Usb.d \
./Driver/timer.d \
./Driver/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Driver/%.o: ../Driver/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -ffunction-sections -fdata-sections  -g3 -DSTM32F10X_HD -DDEBUG -DHSE_VALUE=8000000 -DF_CPU=72000000 -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


