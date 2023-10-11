################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/ano.c \
../user/balance.c \
../user/cpu0_main.c \
../user/cpu1_main.c \
../user/isr.c 

COMPILED_SRCS += \
./user/ano.src \
./user/balance.src \
./user/cpu0_main.src \
./user/cpu1_main.src \
./user/isr.src 

C_DEPS += \
./user/ano.d \
./user/balance.d \
./user/cpu0_main.d \
./user/cpu1_main.d \
./user/isr.d 

OBJS += \
./user/ano.o \
./user/balance.o \
./user/cpu0_main.o \
./user/cpu1_main.o \
./user/isr.o 


# Each subdirectory must supply rules for building sources it contributes
user/%.src: ../user/%.c user/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/smartCar/TC264/exx/tc264_O_car_main_board_demo/E05_03_imu660ra_demo/Release/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O2 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/user\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo '已结束构建： $<'
	@echo ' '

user/%.o: ./user/%.src user/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


clean: clean-user

clean-user:
	-$(RM) ./user/ano.d ./user/ano.o ./user/ano.src ./user/balance.d ./user/balance.o ./user/balance.src ./user/cpu0_main.d ./user/cpu0_main.o ./user/cpu0_main.src ./user/cpu1_main.d ./user/cpu1_main.o ./user/cpu1_main.src ./user/isr.d ./user/isr.o ./user/isr.src

.PHONY: clean-user

