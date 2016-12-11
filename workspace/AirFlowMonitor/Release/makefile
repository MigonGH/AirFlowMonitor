################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
AirFlowMonitor.lss \

FLASH_IMAGE += \
AirFlowMonitor.hex \

EEPROM_IMAGE += \
AirFlowMonitor.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: AirFlowMonitor.elf secondary-outputs

# Tool invocations
AirFlowMonitor.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,AirFlowMonitor.map -mmcu=atmega168 -o "AirFlowMonitor.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

AirFlowMonitor.lss: AirFlowMonitor.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S AirFlowMonitor.elf  >"AirFlowMonitor.lss"
	@echo 'Finished building: $@'
	@echo ' '

AirFlowMonitor.hex: AirFlowMonitor.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex AirFlowMonitor.elf  "AirFlowMonitor.hex"
	@echo 'Finished building: $@'
	@echo ' '

AirFlowMonitor.eep: AirFlowMonitor.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex AirFlowMonitor.elf  "AirFlowMonitor.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: AirFlowMonitor.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega168 AirFlowMonitor.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) AirFlowMonitor.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
