################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/infineon_libraries/iLLD/TC26B/Tricore/Psi5/Std/IfxPsi5.c 

OBJS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Psi5/Std/IfxPsi5.o 

C_DEPS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Psi5/Std/IfxPsi5.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/infineon_libraries/iLLD/TC26B/Tricore/Psi5/Std/%.o: ../libraries/infineon_libraries/iLLD/TC26B/Tricore/Psi5/Std/%.c libraries/infineon_libraries/iLLD/TC26B/Tricore/Psi5/Std/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： AURIX GCC Compiler'
	tricore-gcc -std=c99 "@D:/smartCar/TC264/exx/tc264_O_car_main_board_demo/E05_03_imu660ra_demo/External GCC - Debug/AURIX_GCC_Compiler-Include_paths__-I_.opt" -O0 -g3 -Wall -c -fmessage-length=0 -fno-common -fstrict-volatile-bitfields -fdata-sections -ffunction-sections -mtc161 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


