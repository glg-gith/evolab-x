################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/LabRngUtils.cpp \
../src/LabSet.cpp \
../src/LabTimeUtils.cpp \
../src/TestSets.cpp 

OBJS += \
./src/LabRngUtils.o \
./src/LabSet.o \
./src/LabTimeUtils.o \
./src/TestSets.o 

CPP_DEPS += \
./src/LabRngUtils.d \
./src/LabSet.d \
./src/LabTimeUtils.d \
./src/TestSets.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/golgauth/Documents/02_DEV/EVOLAB/ECLIPSE-WS_ELP/TestSets/src" -O3 -Wall -c -fmessage-length=0 -m64 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


