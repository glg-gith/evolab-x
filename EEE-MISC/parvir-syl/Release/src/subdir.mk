################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/help.cpp \
../src/parsecl.cpp \
../src/parvir.cpp \
../src/utils.cpp \
../src/version.cpp 

OBJS += \
./src/help.o \
./src/parsecl.o \
./src/parvir.o \
./src/utils.o \
./src/version.o 

CPP_DEPS += \
./src/help.d \
./src/parsecl.d \
./src/parvir.d \
./src/utils.d \
./src/version.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -lm -ffast-math -funroll-loops -fomit-frame-pointer -finline -finline-functions -funswitch-loops -lcblas -latlas -fno-math-errno -mfpmath=sse  -Ofast -march=native -funroll-all-loops -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


