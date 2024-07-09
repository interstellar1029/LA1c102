#
# Auto-generated file. Do not edit!
#

GCC_SPECS := D:/LoongIDE/la32-tool/loongarch32-newlib-elf/ls1c102
OS := bare

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include objects.mk
-include subdir.mk
-include core/subdir.mk
-include libc/subdir.mk
-include libc/ctype/subdir.mk
-include libc/errno/subdir.mk
-include libc/stdio/subdir.mk
-include libc/stdlib/subdir.mk
-include libc/string/subdir.mk
-include libc/time/subdir.mk
-include ls1c102/drivers/console/subdir.mk
-include ls1c102/drivers/uart/subdir.mk
-include ls1c102/misc/subdir.mk
-include src/AS608/subdir.mk
-include src/ESP8266/subdir.mk
-include src/GPIO/subdir.mk
-include src/KEY/subdir.mk
-include src/OLED/subdir.mk
-include src/SPI/subdir.mk
-include src/Step_Motor/subdir.mk
-include src/WDG/subdir.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
endif

-include ../makefile.defs

OUT_ELF=test1c102.exe
OUT_MAP=test1c102.map
OUT_BIN=test1c102.bin

LINKCMDS=../ld.script

# Add inputs and outputs from these tool invocations to the build variables

# All Target
all: $(OUT_ELF)

# Tool invocations
$(OUT_ELF): $(STARTO) $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: LoongArch32 ELF C Linker'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-gcc.exe  -mabi=ilp32s -march=loongarch32 -G0 -nostartfiles -nodefaultlibs -nostdlib -static -T $(LINKCMDS) -o $(OUT_ELF) $(STARTO) $(OBJS) $(USER_OBJS) $(LIBS) 
	@echo 'Invoking: LoongArch32 ELF C Objcopy'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-objcopy.exe -O binary $(OUT_ELF) $(OUT_BIN)
	@echo 'Invoking: LoongArch32 ELF C Size'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-size.exe $(OUT_ELF)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(STARTO) $(OBJS) $(C_DEPS) $(CPP_DEPS) $(ASM_DEPS) $(EXECUTABLES) $(OUT_ELF) $(OUT_MAP) $(OUT_BIN)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets


