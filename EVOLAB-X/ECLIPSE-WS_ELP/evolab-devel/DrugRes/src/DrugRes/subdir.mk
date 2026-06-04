################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DrugRes/DrugRes.cpp 

OBJS += \
./src/DrugRes/DrugRes.o 

CPP_DEPS += \
./src/DrugRes/DrugRes.d 


# Each subdirectory must supply rules for building sources it contributes
src/DrugRes/DrugRes.o: ../src/DrugRes/DrugRes.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I../../../DEPENDENCIES/gsl-1.15 -I../../../DEPENDENCIES/libxml2-2.9.0/include -I/usr/include/libxml2 -I../inc -O3 -Wall -c -fmessage-length=0 -m64 -msse4.2 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/DrugRes/DrugRes.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


