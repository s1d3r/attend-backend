################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thirdparty/Crow-master/examples/example.cpp \
../thirdparty/Crow-master/examples/example_blueprint.cpp \
../thirdparty/Crow-master/examples/example_catchall.cpp \
../thirdparty/Crow-master/examples/example_chat.cpp \
../thirdparty/Crow-master/examples/example_compression.cpp \
../thirdparty/Crow-master/examples/example_json_map.cpp \
../thirdparty/Crow-master/examples/example_middleware.cpp \
../thirdparty/Crow-master/examples/example_static_file.cpp \
../thirdparty/Crow-master/examples/example_vs.cpp \
../thirdparty/Crow-master/examples/example_with_all.cpp \
../thirdparty/Crow-master/examples/helloworld.cpp 

CPP_DEPS += \
./thirdparty/Crow-master/examples/example.d \
./thirdparty/Crow-master/examples/example_blueprint.d \
./thirdparty/Crow-master/examples/example_catchall.d \
./thirdparty/Crow-master/examples/example_chat.d \
./thirdparty/Crow-master/examples/example_compression.d \
./thirdparty/Crow-master/examples/example_json_map.d \
./thirdparty/Crow-master/examples/example_middleware.d \
./thirdparty/Crow-master/examples/example_static_file.d \
./thirdparty/Crow-master/examples/example_vs.d \
./thirdparty/Crow-master/examples/example_with_all.d \
./thirdparty/Crow-master/examples/helloworld.d 

OBJS += \
./thirdparty/Crow-master/examples/example.o \
./thirdparty/Crow-master/examples/example_blueprint.o \
./thirdparty/Crow-master/examples/example_catchall.o \
./thirdparty/Crow-master/examples/example_chat.o \
./thirdparty/Crow-master/examples/example_compression.o \
./thirdparty/Crow-master/examples/example_json_map.o \
./thirdparty/Crow-master/examples/example_middleware.o \
./thirdparty/Crow-master/examples/example_static_file.o \
./thirdparty/Crow-master/examples/example_vs.o \
./thirdparty/Crow-master/examples/example_with_all.o \
./thirdparty/Crow-master/examples/helloworld.o 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/Crow-master/examples/%.o: ../thirdparty/Crow-master/examples/%.cpp thirdparty/Crow-master/examples/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/mr_s/Workspace/Cpp/AttendServer/thirdparty/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-thirdparty-2f-Crow-2d-master-2f-examples

clean-thirdparty-2f-Crow-2d-master-2f-examples:
	-$(RM) ./thirdparty/Crow-master/examples/example.d ./thirdparty/Crow-master/examples/example.o ./thirdparty/Crow-master/examples/example_blueprint.d ./thirdparty/Crow-master/examples/example_blueprint.o ./thirdparty/Crow-master/examples/example_catchall.d ./thirdparty/Crow-master/examples/example_catchall.o ./thirdparty/Crow-master/examples/example_chat.d ./thirdparty/Crow-master/examples/example_chat.o ./thirdparty/Crow-master/examples/example_compression.d ./thirdparty/Crow-master/examples/example_compression.o ./thirdparty/Crow-master/examples/example_json_map.d ./thirdparty/Crow-master/examples/example_json_map.o ./thirdparty/Crow-master/examples/example_middleware.d ./thirdparty/Crow-master/examples/example_middleware.o ./thirdparty/Crow-master/examples/example_static_file.d ./thirdparty/Crow-master/examples/example_static_file.o ./thirdparty/Crow-master/examples/example_vs.d ./thirdparty/Crow-master/examples/example_vs.o ./thirdparty/Crow-master/examples/example_with_all.d ./thirdparty/Crow-master/examples/example_with_all.o ./thirdparty/Crow-master/examples/helloworld.d ./thirdparty/Crow-master/examples/helloworld.o

.PHONY: clean-thirdparty-2f-Crow-2d-master-2f-examples

