################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/infineon_libraries/iLLD/TC26B/Tricore/Asclin/Lin/IfxAsclin_Lin.c 

OBJS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Asclin/Lin/IfxAsclin_Lin.o 

C_DEPS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Asclin/Lin/IfxAsclin_Lin.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/infineon_libraries/iLLD/TC26B/Tricore/Asclin/Lin/%.o: ../libraries/infineon_libraries/iLLD/TC26B/Tricore/Asclin/Lin/%.c libraries/infineon_libraries/iLLD/TC26B/Tricore/Asclin/Lin/subdir.mk
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� AURIX GCC Compiler'
	tricore-gcc -std=c99 "@D:/smartCar/TC264/exx/tc264_O_car_main_board_demo/E05_03_imu660ra_demo/External GCC - Debug/AURIX_GCC_Compiler-Include_paths__-I_.opt" -O0 -g3 -Wall -c -fmessage-length=0 -fno-common -fstrict-volatile-bitfields -fdata-sections -ffunction-sections -mtc161 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo '�ѽ��������� $<'
	@echo ' '


