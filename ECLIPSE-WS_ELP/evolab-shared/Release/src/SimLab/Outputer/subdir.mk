################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SimLab/Outputer/LabOutputer.cpp \
../src/SimLab/Outputer/LabOutputerLine.cpp 

OBJS += \
./src/SimLab/Outputer/LabOutputer.o \
./src/SimLab/Outputer/LabOutputerLine.o 

CPP_DEPS += \
./src/SimLab/Outputer/LabOutputer.d \
./src/SimLab/Outputer/LabOutputerLine.d 


# Each subdirectory must supply rules for building sources it contributes
src/SimLab/Outputer/%.o: ../src/SimLab/Outputer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include/python2.7 -I../../../DEPENDENCIES/gsl-1.15 -I../../../DEPENDENCIES/libxml2-2.9.0/include -I/usr/include/libxml2 -I../src/Utils/tclap -I../src/Math -I../src/Utils -I../src/Network -I../src/SimLab/SimCore -I../src/SimLab/Inputer -I../src/SimLab/Outputer -I../src/SimStudy -O3 -Wall -c -fmessage-length=0 -fPIE -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


