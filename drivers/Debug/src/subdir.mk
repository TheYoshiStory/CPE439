################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/gpio.c \
../src/main.c \
../src/motor.c \
../src/odometer.c \
../src/platform.c \
../src/pwm.c \
../src/rangefinder.c 

OBJS += \
./src/gpio.o \
./src/main.o \
./src/motor.o \
./src/odometer.o \
./src/platform.o \
./src/pwm.o \
./src/rangefinder.o 

C_DEPS += \
./src/gpio.d \
./src/main.d \
./src/motor.d \
./src/odometer.d \
./src/platform.d \
./src/pwm.d \
./src/rangefinder.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -IC:/Users/eradb/CPE439/final_project/final_project.sdk/standalone_bsp_0/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


