################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Spatial/BdmLsim.cpp \
../src/Spatial/BdmLsim3.cpp 

OBJS += \
./src/Spatial/BdmLsim.o \
./src/Spatial/BdmLsim3.o 

CPP_DEPS += \
./src/Spatial/BdmLsim.d \
./src/Spatial/BdmLsim3.d 


# Each subdirectory must supply rules for building sources it contributes
src/Spatial/%.o: ../src/Spatial/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/golgauth/Documents/02_DEV/EVOLAB/ECLIPSE-WS_ELP/evolab-devel/inc" -I/usr/include/libxml2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


