################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Network/Network.cpp 

OBJS += \
./src/Network/Network.o 

CPP_DEPS += \
./src/Network/Network.d 


# Each subdirectory must supply rules for building sources it contributes
src/Network/%.o: ../src/Network/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D_RUN_UNDER_ECLIPSE=1 -I"/home/golgauth/Documents/02_DEV/ECLIPSE-WS_ELP/evolab-project/src/Utils" -I"/home/golgauth/Documents/02_DEV/ECLIPSE-WS_ELP/evolab-project/src/Network" -I"/home/golgauth/Documents/02_DEV/ECLIPSE-WS_ELP/evolab-project/src/Simulations" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


