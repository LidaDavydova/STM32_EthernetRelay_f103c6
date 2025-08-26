################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/HttpConfig/config.c 

OBJS += \
./Core/HttpConfig/config.o 

C_DEPS += \
./Core/HttpConfig/config.d 


# Each subdirectory must supply rules for building sources it contributes
Core/HttpConfig/%.o Core/HttpConfig/%.su Core/HttpConfig/%.cyclo: ../Core/HttpConfig/%.c Core/HttpConfig/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103x6 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/CheckRAMSize -I../Core/Ethernet -I../Core/HttpLogger -I../Core/HttpConfig -I../Core/DahuaEvents -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-HttpConfig

clean-Core-2f-HttpConfig:
	-$(RM) ./Core/HttpConfig/config.cyclo ./Core/HttpConfig/config.d ./Core/HttpConfig/config.o ./Core/HttpConfig/config.su

.PHONY: clean-Core-2f-HttpConfig

