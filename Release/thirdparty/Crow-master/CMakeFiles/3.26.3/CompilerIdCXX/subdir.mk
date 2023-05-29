################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thirdparty/Crow-master/CMakeFiles/3.26.3/CompilerIdCXX/CMakeCXXCompilerId.cpp 

CPP_DEPS += \
./thirdparty/Crow-master/CMakeFiles/3.26.3/CompilerIdCXX/CMakeCXXCompilerId.d 

OBJS += \
./thirdparty/Crow-master/CMakeFiles/3.26.3/CompilerIdCXX/CMakeCXXCompilerId.o 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/Crow-master/CMakeFiles/3.26.3/CompilerIdCXX/%.o: ../thirdparty/Crow-master/CMakeFiles/3.26.3/CompilerIdCXX/%.cpp thirdparty/Crow-master/CMakeFiles/3.26.3/CompilerIdCXX/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/mr_s/Workspace/Cpp/AttendServer/thirdparty/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-thirdparty-2f-Crow-2d-master-2f-CMakeFiles-2f-3-2e-26-2e-3-2f-CompilerIdCXX

clean-thirdparty-2f-Crow-2d-master-2f-CMakeFiles-2f-3-2e-26-2e-3-2f-CompilerIdCXX:
	-$(RM) ./thirdparty/Crow-master/CMakeFiles/3.26.3/CompilerIdCXX/CMakeCXXCompilerId.d ./thirdparty/Crow-master/CMakeFiles/3.26.3/CompilerIdCXX/CMakeCXXCompilerId.o

.PHONY: clean-thirdparty-2f-Crow-2d-master-2f-CMakeFiles-2f-3-2e-26-2e-3-2f-CompilerIdCXX

