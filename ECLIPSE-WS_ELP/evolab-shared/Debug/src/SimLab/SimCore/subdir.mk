################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SimLab/SimCore/LabActionStack.cpp \
../src/SimLab/SimCore/LabArrayBinding.cpp \
../src/SimLab/SimCore/LabAttributesStack.cpp \
../src/SimLab/SimCore/LabEventStack.cpp \
../src/SimLab/SimCore/LabNetBinding.cpp \
../src/SimLab/SimCore/LabSimulatorArray.cpp \
../src/SimLab/SimCore/LabSimulatorBase.cpp \
../src/SimLab/SimCore/LabSimulatorNet.cpp \
../src/SimLab/SimCore/LabSimulatorTime.cpp \
../src/SimLab/SimCore/LabSiteBase.cpp \
../src/SimLab/SimCore/LabSiteEvent.cpp \
../src/SimLab/SimCore/LabStateStack.cpp \
../src/SimLab/SimCore/LabUserData.cpp \
../src/SimLab/SimCore/TestClass.cpp 

OBJS += \
./src/SimLab/SimCore/LabActionStack.o \
./src/SimLab/SimCore/LabArrayBinding.o \
./src/SimLab/SimCore/LabAttributesStack.o \
./src/SimLab/SimCore/LabEventStack.o \
./src/SimLab/SimCore/LabNetBinding.o \
./src/SimLab/SimCore/LabSimulatorArray.o \
./src/SimLab/SimCore/LabSimulatorBase.o \
./src/SimLab/SimCore/LabSimulatorNet.o \
./src/SimLab/SimCore/LabSimulatorTime.o \
./src/SimLab/SimCore/LabSiteBase.o \
./src/SimLab/SimCore/LabSiteEvent.o \
./src/SimLab/SimCore/LabStateStack.o \
./src/SimLab/SimCore/LabUserData.o \
./src/SimLab/SimCore/TestClass.o 

CPP_DEPS += \
./src/SimLab/SimCore/LabActionStack.d \
./src/SimLab/SimCore/LabArrayBinding.d \
./src/SimLab/SimCore/LabAttributesStack.d \
./src/SimLab/SimCore/LabEventStack.d \
./src/SimLab/SimCore/LabNetBinding.d \
./src/SimLab/SimCore/LabSimulatorArray.d \
./src/SimLab/SimCore/LabSimulatorBase.d \
./src/SimLab/SimCore/LabSimulatorNet.d \
./src/SimLab/SimCore/LabSimulatorTime.d \
./src/SimLab/SimCore/LabSiteBase.d \
./src/SimLab/SimCore/LabSiteEvent.d \
./src/SimLab/SimCore/LabStateStack.d \
./src/SimLab/SimCore/LabUserData.d \
./src/SimLab/SimCore/TestClass.d 


# Each subdirectory must supply rules for building sources it contributes
src/SimLab/SimCore/%.o: ../src/SimLab/SimCore/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I../../../DEPENDENCIES/gsl-1.15 -I../../../DEPENDENCIES/libxml2-2.9.0/include -I../src/Utils/tclap -I../src/Math -I../src/Utils -I../src/Network -I../src/SimLab/SimCore -I../src/SimLab/Inputer -I../src/SimLab/Outputer -I../src/SimStudy -I/usr/include/libxml2 -O2 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


