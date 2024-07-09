#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../ls1c102/misc/fls.c \
../ls1c102/misc/print_hex.c \
../ls1c102/misc/qsort.c

OBJS += \
./ls1c102/misc/fls.o \
./ls1c102/misc/print_hex.o \
./ls1c102/misc/qsort.o

C_DEPS += \
./ls1c102/misc/fls.d \
./ls1c102/misc/print_hex.d \
./ls1c102/misc/qsort.d

# Each subdirectory must supply rules for building sources it contributes
ls1c102/misc/%.o: ../ls1c102/misc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: LoongArch32 ELF C Compiler'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-gcc.exe -mabi=ilp32s -march=loongarch32 -G0 -DLS1c102 -DOS_NONE  -O1 -fno-builtin -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../include" -I"../ls1c102/include" -I"../ls1c102/drivers/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

