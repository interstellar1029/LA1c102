#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../libc/stdlib/atoi.c \
../libc/stdlib/atol.c \
../libc/stdlib/strtol.c \
../libc/stdlib/strtoul.c

OBJS += \
./libc/stdlib/atoi.o \
./libc/stdlib/atol.o \
./libc/stdlib/strtol.o \
./libc/stdlib/strtoul.o

C_DEPS += \
./libc/stdlib/atoi.d \
./libc/stdlib/atol.d \
./libc/stdlib/strtol.d \
./libc/stdlib/strtoul.d

# Each subdirectory must supply rules for building sources it contributes
libc/stdlib/%.o: ../libc/stdlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: LoongArch32 ELF C Compiler'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-gcc.exe -mabi=ilp32s -march=loongarch32 -G0 -DLS1c102 -DOS_NONE  -O1 -fno-builtin -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../include" -I"../ls1c102/include" -I"../ls1c102/drivers/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

