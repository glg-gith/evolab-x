################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/gsl_rng/error.c \
../src/gsl_rng/gfsr4.c \
../src/gsl_rng/inline.c \
../src/gsl_rng/message.c \
../src/gsl_rng/mt.c \
../src/gsl_rng/rng.c \
../src/gsl_rng/stream.c \
../src/gsl_rng/taus.c 

OBJS += \
./src/gsl_rng/error.o \
./src/gsl_rng/gfsr4.o \
./src/gsl_rng/inline.o \
./src/gsl_rng/message.o \
./src/gsl_rng/mt.o \
./src/gsl_rng/rng.o \
./src/gsl_rng/stream.o \
./src/gsl_rng/taus.o 

C_DEPS += \
./src/gsl_rng/error.d \
./src/gsl_rng/gfsr4.d \
./src/gsl_rng/inline.d \
./src/gsl_rng/message.d \
./src/gsl_rng/mt.d \
./src/gsl_rng/rng.d \
./src/gsl_rng/stream.d \
./src/gsl_rng/taus.d 


# Each subdirectory must supply rules for building sources it contributes
src/gsl_rng/%.o: ../src/gsl_rng/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	icc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


