################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/zf_driver/zf_driver_adc.c \
../libraries/zf_driver/zf_driver_delay.c \
../libraries/zf_driver/zf_driver_dma.c \
../libraries/zf_driver/zf_driver_encoder.c \
../libraries/zf_driver/zf_driver_exti.c \
../libraries/zf_driver/zf_driver_flash.c \
../libraries/zf_driver/zf_driver_gpio.c \
../libraries/zf_driver/zf_driver_pit.c \
../libraries/zf_driver/zf_driver_pwm.c \
../libraries/zf_driver/zf_driver_soft_iic.c \
../libraries/zf_driver/zf_driver_soft_spi.c \
../libraries/zf_driver/zf_driver_spi.c \
../libraries/zf_driver/zf_driver_timer.c \
../libraries/zf_driver/zf_driver_uart.c 

OBJS += \
./libraries/zf_driver/zf_driver_adc.o \
./libraries/zf_driver/zf_driver_delay.o \
./libraries/zf_driver/zf_driver_dma.o \
./libraries/zf_driver/zf_driver_encoder.o \
./libraries/zf_driver/zf_driver_exti.o \
./libraries/zf_driver/zf_driver_flash.o \
./libraries/zf_driver/zf_driver_gpio.o \
./libraries/zf_driver/zf_driver_pit.o \
./libraries/zf_driver/zf_driver_pwm.o \
./libraries/zf_driver/zf_driver_soft_iic.o \
./libraries/zf_driver/zf_driver_soft_spi.o \
./libraries/zf_driver/zf_driver_spi.o \
./libraries/zf_driver/zf_driver_timer.o \
./libraries/zf_driver/zf_driver_uart.o 

C_DEPS += \
./libraries/zf_driver/zf_driver_adc.d \
./libraries/zf_driver/zf_driver_delay.d \
./libraries/zf_driver/zf_driver_dma.d \
./libraries/zf_driver/zf_driver_encoder.d \
./libraries/zf_driver/zf_driver_exti.d \
./libraries/zf_driver/zf_driver_flash.d \
./libraries/zf_driver/zf_driver_gpio.d \
./libraries/zf_driver/zf_driver_pit.d \
./libraries/zf_driver/zf_driver_pwm.d \
./libraries/zf_driver/zf_driver_soft_iic.d \
./libraries/zf_driver/zf_driver_soft_spi.d \
./libraries/zf_driver/zf_driver_spi.d \
./libraries/zf_driver/zf_driver_timer.d \
./libraries/zf_driver/zf_driver_uart.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/zf_driver/%.o: ../libraries/zf_driver/%.c libraries/zf_driver/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： AURIX GCC Compiler'
	tricore-gcc -std=c99 "@D:/smartCar/TC264/exx/tc264_O_car_main_board_demo/E05_03_imu660ra_demo/External GCC - Release/AURIX_GCC_Compiler-Include_paths__-I_.opt" -O3 -Wall -c -fmessage-length=0 -fno-common -fstrict-volatile-bitfields -fdata-sections -ffunction-sections -mtc161 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


