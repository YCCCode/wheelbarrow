################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/infineon_libraries/iLLD/TC26B/Tricore/Gpt12/Std/IfxGpt12.c 

COMPILED_SRCS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Gpt12/Std/IfxGpt12.src 

C_DEPS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Gpt12/Std/IfxGpt12.d 

OBJS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Gpt12/Std/IfxGpt12.o 


# Each subdirectory must supply rules for building sources it contributes
libraries/infineon_libraries/iLLD/TC26B/Tricore/Gpt12/Std/%.src: ../libraries/infineon_libraries/iLLD/TC26B/Tricore/Gpt12/Std/%.c libraries/infineon_libraries/iLLD/TC26B/Tricore/Gpt12/Std/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/smartCar/TC264/exx/tc264_O_car_main_board_demo/E05_03_imu660ra_demo/Release/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O2 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/libraries\/infineon_libraries\/iLLD\/TC26B\/Tricore\/Gpt12\/Std\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo '已结束构建： $<'
	@echo ' '

libraries/infineon_libraries/iLLD/TC26B/Tricore/Gpt12/Std/%.o: ./libraries/infineon_libraries/iLLD/TC26B/Tricore/Gpt12/Std/%.src libraries/infineon_libraries/iLLD/TC26B/Tricore/Gpt12/Std/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


clean: clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC26B-2f-Tricore-2f-Gpt12-2f-Std

clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC26B-2f-Tricore-2f-Gpt12-2f-Std:
	-$(RM) ./libraries/infineon_libraries/iLLD/TC26B/Tricore/Gpt12/Std/IfxGpt12.d ./libraries/infineon_libraries/iLLD/TC26B/Tricore/Gpt12/Std/IfxGpt12.o ./libraries/infineon_libraries/iLLD/TC26B/Tricore/Gpt12/Std/IfxGpt12.src

.PHONY: clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC26B-2f-Tricore-2f-Gpt12-2f-Std

