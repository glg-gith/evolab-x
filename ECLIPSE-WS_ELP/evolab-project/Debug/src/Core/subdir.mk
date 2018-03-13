################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Core/LabConsole.cpp \
../src/Core/LabEventManager.cpp \
../src/Core/LabLauncher.cpp \
../src/Core/LabLibLoader.cpp \
../src/Core/LabPluginsManager.cpp \
../src/Core/LabProcessor.cpp \
../src/Core/LabUtils.cpp \
../src/Core/LabVarManager.cpp \
../src/Core/main.cpp 

OBJS += \
./src/Core/LabConsole.o \
./src/Core/LabEventManager.o \
./src/Core/LabLauncher.o \
./src/Core/LabLibLoader.o \
./src/Core/LabPluginsManager.o \
./src/Core/LabProcessor.o \
./src/Core/LabUtils.o \
./src/Core/LabVarManager.o \
./src/Core/main.o 

CPP_DEPS += \
./src/Core/LabConsole.d \
./src/Core/LabEventManager.d \
./src/Core/LabLauncher.d \
./src/Core/LabLibLoader.d \
./src/Core/LabPluginsManager.d \
./src/Core/LabProcessor.d \
./src/Core/LabUtils.d \
./src/Core/LabVarManager.d \
./src/Core/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/Core/%.o: ../src/Core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D_RUN_UNDER_ECLIPSE=1 -I"/home/golgauth/Documents/02_DEV/ECLIPSE-WS_ELP/evolab-project/src/Utils" -I"/home/golgauth/Documents/02_DEV/ECLIPSE-WS_ELP/evolab-project/src/Network" -I"/home/golgauth/Documents/02_DEV/ECLIPSE-WS_ELP/evolab-project/src/Simulations" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


