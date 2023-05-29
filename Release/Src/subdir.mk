################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/crow.cpp \
../Src/main.cpp 

CPP_DEPS += \
./Src/crow.d \
./Src/main.d 

OBJS += \
./Src/crow.o \
./Src/main.o 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.cpp Src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/mr_s/Workspace/Cpp/AttendServer/thirdparty/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-Src

clean-Src:
	-$(RM) ./Src/crow.d ./Src/crow.o ./Src/main.d ./Src/main.o

.PHONY: clean-Src

