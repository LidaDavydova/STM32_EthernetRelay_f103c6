################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/DahuaEvents/client.c \
../Core/DahuaEvents/digest.c \
../Core/DahuaEvents/hash.c \
../Core/DahuaEvents/http_digest.c \
../Core/DahuaEvents/md5.c \
../Core/DahuaEvents/parse.c \
../Core/DahuaEvents/server.c 

OBJS += \
./Core/DahuaEvents/client.o \
./Core/DahuaEvents/digest.o \
./Core/DahuaEvents/hash.o \
./Core/DahuaEvents/http_digest.o \
./Core/DahuaEvents/md5.o \
./Core/DahuaEvents/parse.o \
./Core/DahuaEvents/server.o 

C_DEPS += \
./Core/DahuaEvents/client.d \
./Core/DahuaEvents/digest.d \
./Core/DahuaEvents/hash.d \
./Core/DahuaEvents/http_digest.d \
./Core/DahuaEvents/md5.d \
./Core/DahuaEvents/parse.d \
./Core/DahuaEvents/server.d 


# Each subdirectory must supply rules for building sources it contributes
Core/DahuaEvents/%.o Core/DahuaEvents/%.su Core/DahuaEvents/%.cyclo: ../Core/DahuaEvents/%.c Core/DahuaEvents/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103x6 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/CheckRAMSize -I../Core/Ethernet -I../Core/HttpLogger -I../Core/HttpConfig -I../Core/DahuaEvents -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-DahuaEvents

clean-Core-2f-DahuaEvents:
	-$(RM) ./Core/DahuaEvents/client.cyclo ./Core/DahuaEvents/client.d ./Core/DahuaEvents/client.o ./Core/DahuaEvents/client.su ./Core/DahuaEvents/digest.cyclo ./Core/DahuaEvents/digest.d ./Core/DahuaEvents/digest.o ./Core/DahuaEvents/digest.su ./Core/DahuaEvents/hash.cyclo ./Core/DahuaEvents/hash.d ./Core/DahuaEvents/hash.o ./Core/DahuaEvents/hash.su ./Core/DahuaEvents/http_digest.cyclo ./Core/DahuaEvents/http_digest.d ./Core/DahuaEvents/http_digest.o ./Core/DahuaEvents/http_digest.su ./Core/DahuaEvents/md5.cyclo ./Core/DahuaEvents/md5.d ./Core/DahuaEvents/md5.o ./Core/DahuaEvents/md5.su ./Core/DahuaEvents/parse.cyclo ./Core/DahuaEvents/parse.d ./Core/DahuaEvents/parse.o ./Core/DahuaEvents/parse.su ./Core/DahuaEvents/server.cyclo ./Core/DahuaEvents/server.d ./Core/DahuaEvents/server.o ./Core/DahuaEvents/server.su

.PHONY: clean-Core-2f-DahuaEvents

