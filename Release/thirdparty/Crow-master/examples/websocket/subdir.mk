################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thirdparty/Crow-master/examples/websocket/example_ws.cpp 

CPP_DEPS += \
./thirdparty/Crow-master/examples/websocket/example_ws.d 

OBJS += \
./thirdparty/Crow-master/examples/websocket/example_ws.o 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/Crow-master/examples/websocket/%.o: ../thirdparty/Crow-master/examples/websocket/%.cpp thirdparty/Crow-master/examples/websocket/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/mr_s/Workspace/Cpp/AttendServer/thirdparty/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-thirdparty-2f-Crow-2d-master-2f-examples-2f-websocket

clean-thirdparty-2f-Crow-2d-master-2f-examples-2f-websocket:
	-$(RM) ./thirdparty/Crow-master/examples/websocket/example_ws.d ./thirdparty/Crow-master/examples/websocket/example_ws.o

.PHONY: clean-thirdparty-2f-Crow-2d-master-2f-examples-2f-websocket

