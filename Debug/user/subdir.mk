################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/CAMERA.c \
../user/ImageProcess.c \
../user/ano.c \
../user/attitude.c \
../user/balance.c \
../user/cpu0_main.c \
../user/cpu1_main.c \
../user/image.c \
../user/isr.c 

COMPILED_SRCS += \
./user/CAMERA.src \
./user/ImageProcess.src \
./user/ano.src \
./user/attitude.src \
./user/balance.src \
./user/cpu0_main.src \
./user/cpu1_main.src \
./user/image.src \
./user/isr.src 

C_DEPS += \
./user/CAMERA.d \
./user/ImageProcess.d \
./user/ano.d \
./user/attitude.d \
./user/balance.d \
./user/cpu0_main.d \
./user/cpu1_main.d \
./user/image.d \
./user/isr.d 

OBJS += \
./user/CAMERA.o \
./user/ImageProcess.o \
./user/ano.o \
./user/attitude.o \
./user/balance.o \
./user/cpu0_main.o \
./user/cpu1_main.o \
./user/image.o \
./user/isr.o 


# Each subdirectory must supply rules for building sources it contributes
user/%.src: ../user/%.c user/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/smartcar/back/720/713/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/user\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

user/%.o: ./user/%.src user/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-user

clean-user:
	-$(RM) ./user/CAMERA.d ./user/CAMERA.o ./user/CAMERA.src ./user/ImageProcess.d ./user/ImageProcess.o ./user/ImageProcess.src ./user/ano.d ./user/ano.o ./user/ano.src ./user/attitude.d ./user/attitude.o ./user/attitude.src ./user/balance.d ./user/balance.o ./user/balance.src ./user/cpu0_main.d ./user/cpu0_main.o ./user/cpu0_main.src ./user/cpu1_main.d ./user/cpu1_main.o ./user/cpu1_main.src ./user/image.d ./user/image.o ./user/image.src ./user/isr.d ./user/isr.o ./user/isr.src

.PHONY: clean-user

