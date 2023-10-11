################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/zf_device/zf_device_absolute_encoder.c \
../libraries/zf_device/zf_device_bluetooth_ch9141.c \
../libraries/zf_device/zf_device_camera.c \
../libraries/zf_device/zf_device_dl1a.c \
../libraries/zf_device/zf_device_gps_tau1201.c \
../libraries/zf_device/zf_device_icm20602.c \
../libraries/zf_device/zf_device_imu660ra.c \
../libraries/zf_device/zf_device_imu963ra.c \
../libraries/zf_device/zf_device_ips114.c \
../libraries/zf_device/zf_device_ips200.c \
../libraries/zf_device/zf_device_key.c \
../libraries/zf_device/zf_device_mpu6050.c \
../libraries/zf_device/zf_device_mt9v03x.c \
../libraries/zf_device/zf_device_oled.c \
../libraries/zf_device/zf_device_ov7725.c \
../libraries/zf_device/zf_device_scc8660.c \
../libraries/zf_device/zf_device_tft180.c \
../libraries/zf_device/zf_device_tsl1401.c \
../libraries/zf_device/zf_device_type.c \
../libraries/zf_device/zf_device_virtual_oscilloscope.c \
../libraries/zf_device/zf_device_wifi_uart.c \
../libraries/zf_device/zf_device_wireless_uart.c 

OBJS += \
./libraries/zf_device/zf_device_absolute_encoder.o \
./libraries/zf_device/zf_device_bluetooth_ch9141.o \
./libraries/zf_device/zf_device_camera.o \
./libraries/zf_device/zf_device_dl1a.o \
./libraries/zf_device/zf_device_gps_tau1201.o \
./libraries/zf_device/zf_device_icm20602.o \
./libraries/zf_device/zf_device_imu660ra.o \
./libraries/zf_device/zf_device_imu963ra.o \
./libraries/zf_device/zf_device_ips114.o \
./libraries/zf_device/zf_device_ips200.o \
./libraries/zf_device/zf_device_key.o \
./libraries/zf_device/zf_device_mpu6050.o \
./libraries/zf_device/zf_device_mt9v03x.o \
./libraries/zf_device/zf_device_oled.o \
./libraries/zf_device/zf_device_ov7725.o \
./libraries/zf_device/zf_device_scc8660.o \
./libraries/zf_device/zf_device_tft180.o \
./libraries/zf_device/zf_device_tsl1401.o \
./libraries/zf_device/zf_device_type.o \
./libraries/zf_device/zf_device_virtual_oscilloscope.o \
./libraries/zf_device/zf_device_wifi_uart.o \
./libraries/zf_device/zf_device_wireless_uart.o 

C_DEPS += \
./libraries/zf_device/zf_device_absolute_encoder.d \
./libraries/zf_device/zf_device_bluetooth_ch9141.d \
./libraries/zf_device/zf_device_camera.d \
./libraries/zf_device/zf_device_dl1a.d \
./libraries/zf_device/zf_device_gps_tau1201.d \
./libraries/zf_device/zf_device_icm20602.d \
./libraries/zf_device/zf_device_imu660ra.d \
./libraries/zf_device/zf_device_imu963ra.d \
./libraries/zf_device/zf_device_ips114.d \
./libraries/zf_device/zf_device_ips200.d \
./libraries/zf_device/zf_device_key.d \
./libraries/zf_device/zf_device_mpu6050.d \
./libraries/zf_device/zf_device_mt9v03x.d \
./libraries/zf_device/zf_device_oled.d \
./libraries/zf_device/zf_device_ov7725.d \
./libraries/zf_device/zf_device_scc8660.d \
./libraries/zf_device/zf_device_tft180.d \
./libraries/zf_device/zf_device_tsl1401.d \
./libraries/zf_device/zf_device_type.d \
./libraries/zf_device/zf_device_virtual_oscilloscope.d \
./libraries/zf_device/zf_device_wifi_uart.d \
./libraries/zf_device/zf_device_wireless_uart.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/zf_device/%.o: ../libraries/zf_device/%.c libraries/zf_device/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： AURIX GCC Compiler'
	tricore-gcc -std=c99 "@D:/smartCar/TC264/exx/tc264_O_car_main_board_demo/E05_03_imu660ra_demo/External GCC - Debug/AURIX_GCC_Compiler-Include_paths__-I_.opt" -O0 -g3 -Wall -c -fmessage-length=0 -fno-common -fstrict-volatile-bitfields -fdata-sections -ffunction-sections -mtc161 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


