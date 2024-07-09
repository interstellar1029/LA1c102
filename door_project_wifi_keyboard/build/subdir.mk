#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../main.c

OBJS += \
./main.o

C_DEPS += \
./main.d

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: LoongArch32 ELF C Compiler'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-gcc.exe -mabi=ilp32s -march=loongarch32 -G0 -DLS1c102 -DOS_NONE  -O1 -fno-builtin -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../include" -I"../ls1c102/include" -I"../ls1c102/drivers/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

