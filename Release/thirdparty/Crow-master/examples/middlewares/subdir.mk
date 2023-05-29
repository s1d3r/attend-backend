################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thirdparty/Crow-master/examples/middlewares/example_cookies.cpp \
../thirdparty/Crow-master/examples/middlewares/example_cors.cpp \
../thirdparty/Crow-master/examples/middlewares/example_session.cpp 

CPP_DEPS += \
./thirdparty/Crow-master/examples/middlewares/example_cookies.d \
./thirdparty/Crow-master/examples/middlewares/example_cors.d \
./thirdparty/Crow-master/examples/middlewares/example_session.d 

OBJS += \
./thirdparty/Crow-master/examples/middlewares/example_cookies.o \
./thirdparty/Crow-master/examples/middlewares/example_cors.o \
./thirdparty/Crow-master/examples/middlewares/example_session.o 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/Crow-master/examples/middlewares/%.o: ../thirdparty/Crow-master/examples/middlewares/%.cpp thirdparty/Crow-master/examples/middlewares/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/mr_s/Workspace/Cpp/AttendServer/thirdparty/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-thirdparty-2f-Crow-2d-master-2f-examples-2f-middlewares

clean-thirdparty-2f-Crow-2d-master-2f-examples-2f-middlewares:
	-$(RM) ./thirdparty/Crow-master/examples/middlewares/example_cookies.d ./thirdparty/Crow-master/examples/middlewares/example_cookies.o ./thirdparty/Crow-master/examples/middlewares/example_cors.d ./thirdparty/Crow-master/examples/middlewares/example_cors.o ./thirdparty/Crow-master/examples/middlewares/example_session.d ./thirdparty/Crow-master/examples/middlewares/example_session.o

.PHONY: clean-thirdparty-2f-Crow-2d-master-2f-examples-2f-middlewares

