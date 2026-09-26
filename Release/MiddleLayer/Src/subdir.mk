################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MiddleLayer/Src/PID.c \
../MiddleLayer/Src/clark_transformation.c \
../MiddleLayer/Src/park_transformation.c \
../MiddleLayer/Src/svpwm.c 

OBJS += \
./MiddleLayer/Src/PID.o \
./MiddleLayer/Src/clark_transformation.o \
./MiddleLayer/Src/park_transformation.o \
./MiddleLayer/Src/svpwm.o 

C_DEPS += \
./MiddleLayer/Src/PID.d \
./MiddleLayer/Src/clark_transformation.d \
./MiddleLayer/Src/park_transformation.d \
./MiddleLayer/Src/svpwm.d 


# Each subdirectory must supply rules for building sources it contributes
MiddleLayer/Src/%.o MiddleLayer/Src/%.su MiddleLayer/Src/%.cyclo: ../MiddleLayer/Src/%.c MiddleLayer/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MiddleLayer-2f-Src

clean-MiddleLayer-2f-Src:
	-$(RM) ./MiddleLayer/Src/PID.cyclo ./MiddleLayer/Src/PID.d ./MiddleLayer/Src/PID.o ./MiddleLayer/Src/PID.su ./MiddleLayer/Src/clark_transformation.cyclo ./MiddleLayer/Src/clark_transformation.d ./MiddleLayer/Src/clark_transformation.o ./MiddleLayer/Src/clark_transformation.su ./MiddleLayer/Src/park_transformation.cyclo ./MiddleLayer/Src/park_transformation.d ./MiddleLayer/Src/park_transformation.o ./MiddleLayer/Src/park_transformation.su ./MiddleLayer/Src/svpwm.cyclo ./MiddleLayer/Src/svpwm.d ./MiddleLayer/Src/svpwm.o ./MiddleLayer/Src/svpwm.su

.PHONY: clean-MiddleLayer-2f-Src

