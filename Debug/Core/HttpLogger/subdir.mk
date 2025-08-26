################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/HttpLogger/logger.c 

OBJS += \
./Core/HttpLogger/logger.o 

C_DEPS += \
./Core/HttpLogger/logger.d 


# Each subdirectory must supply rules for building sources it contributes
Core/HttpLogger/%.o Core/HttpLogger/%.su Core/HttpLogger/%.cyclo: ../Core/HttpLogger/%.c Core/HttpLogger/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103x6 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/CheckRAMSize -I../Core/Ethernet -I../Core/HttpLogger -I../Core/HttpConfig -I../Core/DahuaEvents -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-HttpLogger

clean-Core-2f-HttpLogger:
	-$(RM) ./Core/HttpLogger/logger.cyclo ./Core/HttpLogger/logger.d ./Core/HttpLogger/logger.o ./Core/HttpLogger/logger.su

.PHONY: clean-Core-2f-HttpLogger

