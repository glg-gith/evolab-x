################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Spatial/BdmLsim3.cpp 

OBJS += \
./src/Spatial/BdmLsim3.o 

CPP_DEPS += \
./src/Spatial/BdmLsim3.d 


# Each subdirectory must supply rules for building sources it contributes
src/Spatial/%.o: ../src/Spatial/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I../../../DEPENDENCIES/gsl-1.15 -I../../../DEPENDENCIES/libxml2-2.9.0/include -I/usr/include/libxml2 -I../inc -O0 -g -Wall -c -fmessage-length=0 -m64 -msse4.2 -g -rdynamic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


