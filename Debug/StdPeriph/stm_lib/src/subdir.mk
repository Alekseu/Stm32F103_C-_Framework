################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../StdPeriph/stm_lib/src/_sbrk.c \
../StdPeriph/stm_lib/src/_write.c \
../StdPeriph/stm_lib/src/misc.c \
../StdPeriph/stm_lib/src/stm32f10x_adc.c \
../StdPeriph/stm_lib/src/stm32f10x_bkp.c \
../StdPeriph/stm_lib/src/stm32f10x_crc.c \
../StdPeriph/stm_lib/src/stm32f10x_dbgmcu.c \
../StdPeriph/stm_lib/src/stm32f10x_dma.c \
../StdPeriph/stm_lib/src/stm32f10x_exti.c \
../StdPeriph/stm_lib/src/stm32f10x_flash.c \
../StdPeriph/stm_lib/src/stm32f10x_gpio.c \
../StdPeriph/stm_lib/src/stm32f10x_i2c.c \
../StdPeriph/stm_lib/src/stm32f10x_iwdg.c \
../StdPeriph/stm_lib/src/stm32f10x_pwr.c \
../StdPeriph/stm_lib/src/stm32f10x_rcc.c \
../StdPeriph/stm_lib/src/stm32f10x_rtc.c \
../StdPeriph/stm_lib/src/stm32f10x_spi.c \
../StdPeriph/stm_lib/src/stm32f10x_tim.c \
../StdPeriph/stm_lib/src/stm32f10x_usart.c \
../StdPeriph/stm_lib/src/stm32f10x_wwdg.c \
../StdPeriph/stm_lib/src/trace_impl.c 

OBJS += \
./StdPeriph/stm_lib/src/_sbrk.o \
./StdPeriph/stm_lib/src/_write.o \
./StdPeriph/stm_lib/src/misc.o \
./StdPeriph/stm_lib/src/stm32f10x_adc.o \
./StdPeriph/stm_lib/src/stm32f10x_bkp.o \
./StdPeriph/stm_lib/src/stm32f10x_crc.o \
./StdPeriph/stm_lib/src/stm32f10x_dbgmcu.o \
./StdPeriph/stm_lib/src/stm32f10x_dma.o \
./StdPeriph/stm_lib/src/stm32f10x_exti.o \
./StdPeriph/stm_lib/src/stm32f10x_flash.o \
./StdPeriph/stm_lib/src/stm32f10x_gpio.o \
./StdPeriph/stm_lib/src/stm32f10x_i2c.o \
./StdPeriph/stm_lib/src/stm32f10x_iwdg.o \
./StdPeriph/stm_lib/src/stm32f10x_pwr.o \
./StdPeriph/stm_lib/src/stm32f10x_rcc.o \
./StdPeriph/stm_lib/src/stm32f10x_rtc.o \
./StdPeriph/stm_lib/src/stm32f10x_spi.o \
./StdPeriph/stm_lib/src/stm32f10x_tim.o \
./StdPeriph/stm_lib/src/stm32f10x_usart.o \
./StdPeriph/stm_lib/src/stm32f10x_wwdg.o \
./StdPeriph/stm_lib/src/trace_impl.o 

C_DEPS += \
./StdPeriph/stm_lib/src/_sbrk.d \
./StdPeriph/stm_lib/src/_write.d \
./StdPeriph/stm_lib/src/misc.d \
./StdPeriph/stm_lib/src/stm32f10x_adc.d \
./StdPeriph/stm_lib/src/stm32f10x_bkp.d \
./StdPeriph/stm_lib/src/stm32f10x_crc.d \
./StdPeriph/stm_lib/src/stm32f10x_dbgmcu.d \
./StdPeriph/stm_lib/src/stm32f10x_dma.d \
./StdPeriph/stm_lib/src/stm32f10x_exti.d \
./StdPeriph/stm_lib/src/stm32f10x_flash.d \
./StdPeriph/stm_lib/src/stm32f10x_gpio.d \
./StdPeriph/stm_lib/src/stm32f10x_i2c.d \
./StdPeriph/stm_lib/src/stm32f10x_iwdg.d \
./StdPeriph/stm_lib/src/stm32f10x_pwr.d \
./StdPeriph/stm_lib/src/stm32f10x_rcc.d \
./StdPeriph/stm_lib/src/stm32f10x_rtc.d \
./StdPeriph/stm_lib/src/stm32f10x_spi.d \
./StdPeriph/stm_lib/src/stm32f10x_tim.d \
./StdPeriph/stm_lib/src/stm32f10x_usart.d \
./StdPeriph/stm_lib/src/stm32f10x_wwdg.d \
./StdPeriph/stm_lib/src/trace_impl.d 


# Each subdirectory must supply rules for building sources it contributes
StdPeriph/stm_lib/src/%.o: ../StdPeriph/stm_lib/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -ffunction-sections -fdata-sections  -g3 -DSTM32F10X_XL -DDEBUG -DHSE_VALUE=8000000 -DF_CPU=72000000 -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


