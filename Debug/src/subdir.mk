################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/fail.cc \
../src/main.cc \
../src/map.cc \
../src/transform.cc \
../src/vector.cc \
../src/work.cc 

OBJS += \
./src/fail.o \
./src/main.o \
./src/map.o \
./src/transform.o \
./src/vector.o \
./src/work.o 

CC_DEPS += \
./src/fail.d \
./src/main.d \
./src/map.d \
./src/transform.d \
./src/vector.d \
./src/work.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

