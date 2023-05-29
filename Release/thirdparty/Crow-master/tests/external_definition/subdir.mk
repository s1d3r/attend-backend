################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thirdparty/Crow-master/tests/external_definition/main.cpp 

CPP_DEPS += \
./thirdparty/Crow-master/tests/external_definition/main.d 

OBJS += \
./thirdparty/Crow-master/tests/external_definition/main.o 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/Crow-master/tests/external_definition/%.o: ../thirdparty/Crow-master/tests/external_definition/%.cpp thirdparty/Crow-master/tests/external_definition/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/mr_s/Workspace/Cpp/AttendServer/thirdparty/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-thirdparty-2f-Crow-2d-master-2f-tests-2f-external_definition

clean-thirdparty-2f-Crow-2d-master-2f-tests-2f-external_definition:
	-$(RM) ./thirdparty/Crow-master/tests/external_definition/main.d ./thirdparty/Crow-master/tests/external_definition/main.o

.PHONY: clean-thirdparty-2f-Crow-2d-master-2f-tests-2f-external_definition

