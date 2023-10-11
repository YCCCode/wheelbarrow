################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/infineon_libraries/iLLD/TC26B/Tricore/Cpu/CStart/IfxCpu_CStart0.c \
../libraries/infineon_libraries/iLLD/TC26B/Tricore/Cpu/CStart/IfxCpu_CStart1.c 

OBJS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Cpu/CStart/IfxCpu_CStart0.o \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Cpu/CStart/IfxCpu_CStart1.o 

C_DEPS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Cpu/CStart/IfxCpu_CStart0.d \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Cpu/CStart/IfxCpu_CStart1.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/infineon_libraries/iLLD/TC26B/Tricore/Cpu/CStart/%.o: ../libraries/infineon_libraries/iLLD/TC26B/Tricore/Cpu/CStart/%.c libraries/infineon_libraries/iLLD/TC26B/Tricore/Cpu/CStart/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： AURIX GCC Compiler'
	tricore-gcc -std=c99 "@D:/smartCar/TC264/exx/tc264_O_car_main_board_demo/E05_03_imu660ra_demo/External GCC - Debug/AURIX_GCC_Compiler-Include_paths__-I_.opt" -O0 -g3 -Wall -c -fmessage-length=0 -fno-common -fstrict-volatile-bitfields -fdata-sections -ffunction-sections -mtc161 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


