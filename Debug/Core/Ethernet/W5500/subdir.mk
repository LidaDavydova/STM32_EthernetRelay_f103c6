################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Ethernet/W5500/w5500.c 

OBJS += \
./Core/Ethernet/W5500/w5500.o 

C_DEPS += \
./Core/Ethernet/W5500/w5500.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Ethernet/W5500/%.o Core/Ethernet/W5500/%.su Core/Ethernet/W5500/%.cyclo: ../Core/Ethernet/W5500/%.c Core/Ethernet/W5500/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103x6 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/CheckRAMSize -I../Core/Ethernet -I../Core/HttpLogger -I../Core/HttpConfig -I../Core/DahuaEvents -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Ethernet-2f-W5500

clean-Core-2f-Ethernet-2f-W5500:
	-$(RM) ./Core/Ethernet/W5500/w5500.cyclo ./Core/Ethernet/W5500/w5500.d ./Core/Ethernet/W5500/w5500.o ./Core/Ethernet/W5500/w5500.su

.PHONY: clean-Core-2f-Ethernet-2f-W5500

