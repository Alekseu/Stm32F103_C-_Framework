################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include fonts/subdir.mk
-include StdPeriph/usb/src/subdir.mk
-include StdPeriph/stm_lib/src/subdir.mk
-include StdPeriph/newlib/subdir.mk
-include StdPeriph/cmsis_boot/subdir.mk
-include Protocols/commandProcessor/subdir.mk
-include Protocols/Rs485/subdir.mk
-include Protocols/Rs422/subdir.mk
-include Os/subdir.mk
-include Extention/shareStorage/subdir.mk
-include Extention/subdir.mk
-include Driver/usb/subdir.mk
-include Driver/usart/subdir.mk
-include Driver/tim/subdir.mk
-include Driver/systick/subdir.mk
-include Driver/spi/subdir.mk
-include Driver/sdio/subdir.mk
-include Driver/rtc/subdir.mk
-include Driver/pwm/subdir.mk
-include Driver/ow/subdir.mk
-include Driver/nvic/subdir.mk
-include Driver/led/subdir.mk
-include Driver/i2c/subdir.mk
-include Driver/gpio/subdir.mk
-include Driver/fsmc/subdir.mk
-include Driver/dma/subdir.mk
-include Driver/dac/subdir.mk
-include Driver/can/subdir.mk
-include Driver/adc/subdir.mk
-include Device/w5500/subdir.mk
-include Device/sdCard/src/subdir.mk
-include Device/nrf2401/subdir.mk
-include Device/esp8266/subdir.mk
-include Device/enc28j60/subdir.mk
-include Device/eeprom/subdir.mk
-include Device/SGPIO/subdir.mk
-include Device/RF433/subdir.mk
-include Device/Lcd/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Stm32F103_Firmware_DEV.hex \

SECONDARY_SIZE += \
Stm32F103_Firmware_DEV.siz \


# All Target
all: Stm32F103_Firmware_DEV.elf secondary-outputs

# Tool invocations
Stm32F103_Firmware_DEV.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -ffunction-sections -fdata-sections  -g3 -T "D:\GitRepositories\GitHub\Stm32F103_C-_Framework\ldscripts\F103\libs.ld" -T "D:\GitRepositories\GitHub\Stm32F103_C-_Framework\ldscripts\F103\mem.ld" -T "D:\GitRepositories\GitHub\Stm32F103_C-_Framework\ldscripts\F103\sections.ld" -Xlinker --gc-sections -Wl,-Map,"Stm32F103_Firmware_DEV.map" -o "Stm32F103_Firmware_DEV.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Stm32F103_Firmware_DEV.hex: Stm32F103_Firmware_DEV.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "Stm32F103_Firmware_DEV.elf"  "Stm32F103_Firmware_DEV.hex"
	@echo 'Finished building: $@'
	@echo ' '

Stm32F103_Firmware_DEV.siz: Stm32F103_Firmware_DEV.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "Stm32F103_Firmware_DEV.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) Stm32F103_Firmware_DEV.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
