################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thirdparty/Crow-master/tests/multi_file/main.cpp \
../thirdparty/Crow-master/tests/multi_file/secondary.cpp 

CPP_DEPS += \
./thirdparty/Crow-master/tests/multi_file/main.d \
./thirdparty/Crow-master/tests/multi_file/secondary.d 

OBJS += \
./thirdparty/Crow-master/tests/multi_file/main.o \
./thirdparty/Crow-master/tests/multi_file/secondary.o 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/Crow-master/tests/multi_file/%.o: ../thirdparty/Crow-master/tests/multi_file/%.cpp thirdparty/Crow-master/tests/multi_file/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/mr_s/Workspace/Cpp/AttendServer/thirdparty/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-thirdparty-2f-Crow-2d-master-2f-tests-2f-multi_file

clean-thirdparty-2f-Crow-2d-master-2f-tests-2f-multi_file:
	-$(RM) ./thirdparty/Crow-master/tests/multi_file/main.d ./thirdparty/Crow-master/tests/multi_file/main.o ./thirdparty/Crow-master/tests/multi_file/secondary.d ./thirdparty/Crow-master/tests/multi_file/secondary.o

.PHONY: clean-thirdparty-2f-Crow-2d-master-2f-tests-2f-multi_file

