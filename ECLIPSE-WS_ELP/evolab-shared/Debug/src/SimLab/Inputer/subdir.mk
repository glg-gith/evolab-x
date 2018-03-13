################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SimLab/Inputer/LabCLInterpreter.cpp \
../src/SimLab/Inputer/LabXMLInterpreter.cpp 

OBJS += \
./src/SimLab/Inputer/LabCLInterpreter.o \
./src/SimLab/Inputer/LabXMLInterpreter.o 

CPP_DEPS += \
./src/SimLab/Inputer/LabCLInterpreter.d \
./src/SimLab/Inputer/LabXMLInterpreter.d 


# Each subdirectory must supply rules for building sources it contributes
src/SimLab/Inputer/%.o: ../src/SimLab/Inputer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I../../../DEPENDENCIES/gsl-1.15 -I../../../DEPENDENCIES/libxml2-2.9.0/include -I../src/Utils/tclap -I../src/Math -I../src/Utils -I../src/Network -I../src/SimLab/SimCore -I../src/SimLab/Inputer -I../src/SimLab/Outputer -I../src/SimStudy -I/usr/include/libxml2 -O2 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


