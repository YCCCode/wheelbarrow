################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Console.c \
../libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Shell.c 

OBJS += \
./libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Console.o \
./libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Shell.o 

C_DEPS += \
./libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Console.d \
./libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Shell.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/%.o: ../libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/%.c libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/subdir.mk
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� AURIX GCC Compiler'
	tricore-gcc -std=c99 "@D:/smartCar/TC264/exx/tc264_O_car_main_board_demo/E05_03_imu660ra_demo/External GCC - Release/AURIX_GCC_Compiler-Include_paths__-I_.opt" -O3 -Wall -c -fmessage-length=0 -fno-common -fstrict-volatile-bitfields -fdata-sections -ffunction-sections -mtc161 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo '�ѽ��������� $<'
	@echo ' '


