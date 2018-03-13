################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SimStudy/_Sims.cpp 

OBJS += \
./src/SimStudy/_Sims.o 

CPP_DEPS += \
./src/SimStudy/_Sims.d 


# Each subdirectory must supply rules for building sources it contributes
src/SimStudy/%.o: ../src/SimStudy/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include/python2.7 -I../../../DEPENDENCIES/gsl-1.15 -I../../../DEPENDENCIES/libxml2-2.9.0/include -I/usr/include/libxml2 -I../src/Utils/tclap -I../src/Math -I../src/Utils -I../src/Network -I../src/SimLab/SimCore -I../src/SimLab/Inputer -I../src/SimLab/Outputer -I../src/SimStudy -O3 -Wall -c -fmessage-length=0 -m64 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


