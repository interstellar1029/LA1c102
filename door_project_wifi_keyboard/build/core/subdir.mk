#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
ASM_SRCS += \
../core/irq_s.S \
../core/start.S

C_SRCS += \
../core/bsp_start.c \
../core/exception.c \
../core/irq.c \
../core/tick.c

STARTO += ./core/start.o

OBJS += \
./core/bsp_start.o \
./core/exception.o \
./core/irq.o \
./core/irq_s.o \
./core/tick.o

ASM_DEPS += \
./core/irq_s.d \
./core/start.d

C_DEPS += \
./core/bsp_start.d \
./core/exception.d \
./core/irq.d \
./core/tick.d

# Each subdirectory must supply rules for building sources it contributes
core/%.o: ../core/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: LoongArch32 ELF C Compiler'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-gcc.exe -mabi=ilp32s -march=loongarch32 -G0 -DLS1c102 -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../ls1c102/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

core/%.o: ../core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: LoongArch32 ELF C Compiler'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-gcc.exe -mabi=ilp32s -march=loongarch32 -G0 -DLS1c102 -DOS_NONE  -O1 -fno-builtin -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../include" -I"../ls1c102/include" -I"../ls1c102/drivers/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

