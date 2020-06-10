################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../appli/sub/sub_clignotement_led.c \
../appli/sub/sub_parachute.c \
../appli/sub/sub_send_data.c 

OBJS += \
./appli/sub/sub_clignotement_led.o \
./appli/sub/sub_parachute.o \
./appli/sub/sub_send_data.o 

C_DEPS += \
./appli/sub/sub_clignotement_led.d \
./appli/sub/sub_parachute.d \
./appli/sub/sub_send_data.d 


# Each subdirectory must supply rules for building sources it contributes
appli/sub/%.o: ../appli/sub/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32F1 -DNUCLEO_F103RB -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/VL53L0X" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/MPU6050" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/iks01a1" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/iks01a1/Common" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/iks01a1/hts221" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/iks01a1/lis3mdl" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/iks01a1/lps22hb" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/iks01a1/lps25hb" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/iks01a1/lsm6ds0" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/iks01a1/lsm6ds3" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/tft_ili9341" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/tft_pcd8544" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/MCP23S17" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/hal/inc" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/Common" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/lcd2x16" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/MLX90614" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/MatrixKeyboard" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/bsp/MatrixLed" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/CMSIS/core" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/CMSIS/device" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/middleware/FatFs" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/middleware/FatFs/src" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/lib/middleware/FatFs/src/drivers" -I"C:/Users/Th�o/Documents/GitHub/STM32/flight_controller_v2/appli"  -Og -g3 -Wall -Wextra -Wconversion -fmessage-length=0 -ffunction-sections -fdata-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


