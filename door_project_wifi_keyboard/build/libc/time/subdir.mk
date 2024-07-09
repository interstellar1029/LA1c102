#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../libc/time/gmtime.c \
../libc/time/localtime.c \
../libc/time/mktime.c \
../libc/time/strftime.c \
../libc/time/time.c \
../libc/time/tm_utils.c

OBJS += \
./libc/time/gmtime.o \
./libc/time/localtime.o \
./libc/time/mktime.o \
./libc/time/strftime.o \
./libc/time/time.o \
./libc/time/tm_utils.o

C_DEPS += \
./libc/time/gmtime.d \
./libc/time/localtime.d \
./libc/time/mktime.d \
./libc/time/strftime.d \
./libc/time/time.d \
./libc/time/tm_utils.d

# Each subdirectory must supply rules for building sources it contributes
libc/time/%.o: ../libc/time/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: LoongArch32 ELF C Compiler'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-gcc.exe -mabi=ilp32s -march=loongarch32 -G0 -DLS1c102 -DOS_NONE  -O1 -fno-builtin -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../include" -I"../ls1c102/include" -I"../ls1c102/drivers/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

