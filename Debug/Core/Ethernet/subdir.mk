################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Ethernet/eth_w5500.c \
../Core/Ethernet/socket.c \
../Core/Ethernet/wizchip_conf.c 

OBJS += \
./Core/Ethernet/eth_w5500.o \
./Core/Ethernet/socket.o \
./Core/Ethernet/wizchip_conf.o 

C_DEPS += \
./Core/Ethernet/eth_w5500.d \
./Core/Ethernet/socket.d \
./Core/Ethernet/wizchip_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Ethernet/%.o Core/Ethernet/%.su Core/Ethernet/%.cyclo: ../Core/Ethernet/%.c Core/Ethernet/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103x6 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/CheckRAMSize -I../Core/Ethernet -I../Core/HttpLogger -I../Core/HttpConfig -I../Core/DahuaEvents -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Ethernet

clean-Core-2f-Ethernet:
	-$(RM) ./Core/Ethernet/eth_w5500.cyclo ./Core/Ethernet/eth_w5500.d ./Core/Ethernet/eth_w5500.o ./Core/Ethernet/eth_w5500.su ./Core/Ethernet/socket.cyclo ./Core/Ethernet/socket.d ./Core/Ethernet/socket.o ./Core/Ethernet/socket.su ./Core/Ethernet/wizchip_conf.cyclo ./Core/Ethernet/wizchip_conf.d ./Core/Ethernet/wizchip_conf.o ./Core/Ethernet/wizchip_conf.su

.PHONY: clean-Core-2f-Ethernet

