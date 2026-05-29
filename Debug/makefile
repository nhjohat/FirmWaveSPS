################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
<<<<<<< HEAD
=======
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
<<<<<<< HEAD
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
=======
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


<<<<<<< HEAD
BUILD_ARTIFACT_NAME := mcu_firmware
=======
BUILD_ARTIFACT_NAME := FIRMWAVEMCU
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
<<<<<<< HEAD
mcu_firmware.elf \

MAP_FILES += \
mcu_firmware.map \
=======
FIRMWAVEMCU.elf \

MAP_FILES += \
FIRMWAVEMCU.map \
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
<<<<<<< HEAD
mcu_firmware.list \
=======
FIRMWAVEMCU.list \
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428


# All Target
all: main-build

# Main-build Target
<<<<<<< HEAD
main-build: mcu_firmware.elf secondary-outputs

# Tool invocations
mcu_firmware.elf mcu_firmware.map: $(OBJS) $(USER_OBJS) D:\learn\CE410.Q21\FIRMWAVE_MCU\mcu_firmware\STM32F401CCUX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "mcu_firmware.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\learn\CE410.Q21\FIRMWAVE_MCU\mcu_firmware\STM32F401CCUX_FLASH.ld" --specs=nosys.specs -Wl,-Map="mcu_firmware.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
=======
main-build: FIRMWAVEMCU.elf secondary-outputs

# Tool invocations
FIRMWAVEMCU.elf FIRMWAVEMCU.map: $(OBJS) $(USER_OBJS) D:\learn\CE410.Q21\CODE\FIRMWAVEMCU\STM32F401CCUX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "FIRMWAVEMCU.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\learn\CE410.Q21\CODE\FIRMWAVEMCU\STM32F401CCUX_FLASH.ld" --specs=nosys.specs -Wl,-Map="FIRMWAVEMCU.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

<<<<<<< HEAD
mcu_firmware.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "mcu_firmware.list"
=======
FIRMWAVEMCU.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "FIRMWAVEMCU.list"
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
<<<<<<< HEAD
	-$(RM) default.size.stdout mcu_firmware.elf mcu_firmware.list mcu_firmware.map
=======
	-$(RM) FIRMWAVEMCU.elf FIRMWAVEMCU.list FIRMWAVEMCU.map default.size.stdout
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
