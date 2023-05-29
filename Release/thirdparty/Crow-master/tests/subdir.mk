################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thirdparty/Crow-master/tests/unittest.cpp 

CPP_DEPS += \
./thirdparty/Crow-master/tests/unittest.d 

OBJS += \
./thirdparty/Crow-master/tests/unittest.o 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/Crow-master/tests/%.o: ../thirdparty/Crow-master/tests/%.cpp thirdparty/Crow-master/tests/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/mr_s/Workspace/Cpp/AttendServer/thirdparty/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-thirdparty-2f-Crow-2d-master-2f-tests

clean-thirdparty-2f-Crow-2d-master-2f-tests:
	-$(RM) ./thirdparty/Crow-master/tests/unittest.d ./thirdparty/Crow-master/tests/unittest.o

.PHONY: clean-thirdparty-2f-Crow-2d-master-2f-tests

