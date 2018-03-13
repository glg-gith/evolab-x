################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ELL/LabEllTranslator.cpp 

OBJS += \
./src/ELL/LabEllTranslator.o 

CPP_DEPS += \
./src/ELL/LabEllTranslator.d 


# Each subdirectory must supply rules for building sources it contributes
src/ELL/%.o: ../src/ELL/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D_RUN_UNDER_ECLIPSE -I"/home/golgauth/Documents/02_DEV/EVOLAB/ECLIPSE-WS_ELP/evolab-project/src/Utils" -I"/home/golgauth/Documents/02_DEV/EVOLAB/ECLIPSE-WS_ELP/evolab-project/src/ELL" -I"/home/golgauth/Documents/02_DEV/EVOLAB/ECLIPSE-WS_ELP/evolab-project/src/Console" -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


