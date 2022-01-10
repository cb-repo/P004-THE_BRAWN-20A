################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32X/Lib/usb/cdc/USB_CDC.c 

OBJS += \
./STM32X/Lib/usb/cdc/USB_CDC.o 

C_DEPS += \
./STM32X/Lib/usb/cdc/USB_CDC.d 


# Each subdirectory must supply rules for building sources it contributes
STM32X/Lib/usb/cdc/USB_CDC.o: ../STM32X/Lib/usb/cdc/USB_CDC.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L010x4 -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I../STM32X/Lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32X/Lib/usb/cdc/USB_CDC.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

