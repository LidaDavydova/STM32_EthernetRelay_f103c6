################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/CheckRAMSize/check_RAM_size.c 

OBJS += \
./Core/CheckRAMSize/check_RAM_size.o 

C_DEPS += \
./Core/CheckRAMSize/check_RAM_size.d 


# Each subdirectory must supply rules for building sources it contributes
Core/CheckRAMSize/%.o Core/CheckRAMSize/%.su Core/CheckRAMSize/%.cyclo: ../Core/CheckRAMSize/%.c Core/CheckRAMSize/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103x6 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/CheckRAMSize -I../Core/Ethernet -I../Core/HttpLogger -I../Core/HttpConfig -I../Core/DahuaEvents -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-CheckRAMSize

clean-Core-2f-CheckRAMSize:
	-$(RM) ./Core/CheckRAMSize/check_RAM_size.cyclo ./Core/CheckRAMSize/check_RAM_size.d ./Core/CheckRAMSize/check_RAM_size.o ./Core/CheckRAMSize/check_RAM_size.su

.PHONY: clean-Core-2f-CheckRAMSize

