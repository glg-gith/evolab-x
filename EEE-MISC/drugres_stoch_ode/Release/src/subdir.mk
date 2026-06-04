################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/drugres.cpp \
../src/help.cpp \
../src/parsecl.cpp \
../src/r.cpp \
../src/utils.cpp \
../src/version.cpp 

OBJS += \
./src/drugres.o \
./src/help.o \
./src/parsecl.o \
./src/r.o \
./src/utils.o \
./src/version.o 

CPP_DEPS += \
./src/drugres.d \
./src/help.d \
./src/parsecl.d \
./src/r.d \
./src/utils.d \
./src/version.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I../../DEPENDENCIES/gsl-1.9-installed/include -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


