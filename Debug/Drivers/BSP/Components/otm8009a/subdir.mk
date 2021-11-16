################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/otm8009a/otm8009a.c 

C_DEPS += \
./Drivers/BSP/Components/otm8009a/otm8009a.d 

OBJS += \
./Drivers/BSP/Components/otm8009a/otm8009a.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/otm8009a/%.o: ../Drivers/BSP/Components/otm8009a/%.c Drivers/BSP/Components/otm8009a/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DSTM32_THREAD_SAFE_STRATEGY=2 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/naudotvardis/Workspaces/CubeIDE/magistrantura/rts_semestrinis_cpp/Drivers/BSP" -I../Core/ThreadSafe -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

