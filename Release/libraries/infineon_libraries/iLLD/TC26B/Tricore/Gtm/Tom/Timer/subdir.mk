################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/infineon_libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer/IfxGtm_Tom_Timer.c 

COMPILED_SRCS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer/IfxGtm_Tom_Timer.src 

C_DEPS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer/IfxGtm_Tom_Timer.d 

OBJS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer/IfxGtm_Tom_Timer.o 


# Each subdirectory must supply rules for building sources it contributes
libraries/infineon_libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer/%.src: ../libraries/infineon_libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer/%.c libraries/infineon_libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer/subdir.mk
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/smartCar/TC264/exx/tc264_O_car_main_board_demo/E05_03_imu660ra_demo/Release/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O2 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/libraries\/infineon_libraries\/iLLD\/TC26B\/Tricore\/Gtm\/Tom\/Timer\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo '�ѽ��������� $<'
	@echo ' '

libraries/infineon_libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer/%.o: ./libraries/infineon_libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer/%.src libraries/infineon_libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer/subdir.mk
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo '�ѽ��������� $<'
	@echo ' '


clean: clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC26B-2f-Tricore-2f-Gtm-2f-Tom-2f-Timer

clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC26B-2f-Tricore-2f-Gtm-2f-Tom-2f-Timer:
	-$(RM) ./libraries/infineon_libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer/IfxGtm_Tom_Timer.d ./libraries/infineon_libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer/IfxGtm_Tom_Timer.o ./libraries/infineon_libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer/IfxGtm_Tom_Timer.src

.PHONY: clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC26B-2f-Tricore-2f-Gtm-2f-Tom-2f-Timer

