################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DriveLayer/Src/Hall_Sensor.c \
../DriveLayer/Src/Motor.c \
../DriveLayer/Src/PWM_EG2134.c \
../DriveLayer/Src/RingBuffer.c \
../DriveLayer/Src/UART2.c \
../DriveLayer/Src/angle_get.c 

OBJS += \
./DriveLayer/Src/Hall_Sensor.o \
./DriveLayer/Src/Motor.o \
./DriveLayer/Src/PWM_EG2134.o \
./DriveLayer/Src/RingBuffer.o \
./DriveLayer/Src/UART2.o \
./DriveLayer/Src/angle_get.o 

C_DEPS += \
./DriveLayer/Src/Hall_Sensor.d \
./DriveLayer/Src/Motor.d \
./DriveLayer/Src/PWM_EG2134.d \
./DriveLayer/Src/RingBuffer.d \
./DriveLayer/Src/UART2.d \
./DriveLayer/Src/angle_get.d 


# Each subdirectory must supply rules for building sources it contributes
DriveLayer/Src/%.o DriveLayer/Src/%.su DriveLayer/Src/%.cyclo: ../DriveLayer/Src/%.c DriveLayer/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-DriveLayer-2f-Src

clean-DriveLayer-2f-Src:
	-$(RM) ./DriveLayer/Src/Hall_Sensor.cyclo ./DriveLayer/Src/Hall_Sensor.d ./DriveLayer/Src/Hall_Sensor.o ./DriveLayer/Src/Hall_Sensor.su ./DriveLayer/Src/Motor.cyclo ./DriveLayer/Src/Motor.d ./DriveLayer/Src/Motor.o ./DriveLayer/Src/Motor.su ./DriveLayer/Src/PWM_EG2134.cyclo ./DriveLayer/Src/PWM_EG2134.d ./DriveLayer/Src/PWM_EG2134.o ./DriveLayer/Src/PWM_EG2134.su ./DriveLayer/Src/RingBuffer.cyclo ./DriveLayer/Src/RingBuffer.d ./DriveLayer/Src/RingBuffer.o ./DriveLayer/Src/RingBuffer.su ./DriveLayer/Src/UART2.cyclo ./DriveLayer/Src/UART2.d ./DriveLayer/Src/UART2.o ./DriveLayer/Src/UART2.su ./DriveLayer/Src/angle_get.cyclo ./DriveLayer/Src/angle_get.d ./DriveLayer/Src/angle_get.o ./DriveLayer/Src/angle_get.su

.PHONY: clean-DriveLayer-2f-Src

