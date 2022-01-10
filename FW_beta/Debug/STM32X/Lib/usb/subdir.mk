################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32X/Lib/usb/USB_CTL.c \
../STM32X/Lib/usb/USB_EP.c \
../STM32X/Lib/usb/USB_PCD.c 

OBJS += \
./STM32X/Lib/usb/USB_CTL.o \
./STM32X/Lib/usb/USB_EP.o \
./STM32X/Lib/usb/USB_PCD.o 

C_DEPS += \
./STM32X/Lib/usb/USB_CTL.d \
./STM32X/Lib/usb/USB_EP.d \
./STM32X/Lib/usb/USB_PCD.d 


# Each subdirectory must supply rules for building sources it contributes
STM32X/Lib/usb/USB_CTL.o: ../STM32X/Lib/usb/USB_CTL.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L010x4 -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I../STM32X/Lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32X/Lib/usb/USB_CTL.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32X/Lib/usb/USB_EP.o: ../STM32X/Lib/usb/USB_EP.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L010x4 -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I../STM32X/Lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32X/Lib/usb/USB_EP.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32X/Lib/usb/USB_PCD.o: ../STM32X/Lib/usb/USB_PCD.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L010x4 -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I../STM32X/Lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32X/Lib/usb/USB_PCD.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

