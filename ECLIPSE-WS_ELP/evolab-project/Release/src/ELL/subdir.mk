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
	g++ -I../src/Utils -I../src/ELL -I../src/Console -O3 -Wall -c -fmessage-length=0 -m64 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


