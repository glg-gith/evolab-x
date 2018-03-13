################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Console/LabConsole.cpp \
../src/Console/LabEventManager.cpp \
../src/Console/LabLauncher.cpp \
../src/Console/LabLibLoader.cpp \
../src/Console/LabPluginsManager.cpp \
../src/Console/LabProcessor.cpp \
../src/Console/LabUtils.cpp \
../src/Console/LabVarManager.cpp 

OBJS += \
./src/Console/LabConsole.o \
./src/Console/LabEventManager.o \
./src/Console/LabLauncher.o \
./src/Console/LabLibLoader.o \
./src/Console/LabPluginsManager.o \
./src/Console/LabProcessor.o \
./src/Console/LabUtils.o \
./src/Console/LabVarManager.o 

CPP_DEPS += \
./src/Console/LabConsole.d \
./src/Console/LabEventManager.d \
./src/Console/LabLauncher.d \
./src/Console/LabLibLoader.d \
./src/Console/LabPluginsManager.d \
./src/Console/LabProcessor.d \
./src/Console/LabUtils.d \
./src/Console/LabVarManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/Console/%.o: ../src/Console/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I../src/Utils -I../src/ELL -I../src/Console -O3 -Wall -c -fmessage-length=0 -m64 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


