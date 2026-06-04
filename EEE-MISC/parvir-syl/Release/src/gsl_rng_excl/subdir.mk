################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/gsl_rng_excl/error.c \
../src/gsl_rng_excl/gfsr4.c \
../src/gsl_rng_excl/inline.c \
../src/gsl_rng_excl/message.c \
../src/gsl_rng_excl/mt.c \
../src/gsl_rng_excl/rng.c \
../src/gsl_rng_excl/stream.c \
../src/gsl_rng_excl/taus.c 

OBJS += \
./src/gsl_rng_excl/error.o \
./src/gsl_rng_excl/gfsr4.o \
./src/gsl_rng_excl/inline.o \
./src/gsl_rng_excl/message.o \
./src/gsl_rng_excl/mt.o \
./src/gsl_rng_excl/rng.o \
./src/gsl_rng_excl/stream.o \
./src/gsl_rng_excl/taus.o 

C_DEPS += \
./src/gsl_rng_excl/error.d \
./src/gsl_rng_excl/gfsr4.d \
./src/gsl_rng_excl/inline.d \
./src/gsl_rng_excl/message.d \
./src/gsl_rng_excl/mt.d \
./src/gsl_rng_excl/rng.d \
./src/gsl_rng_excl/stream.d \
./src/gsl_rng_excl/taus.d 


# Each subdirectory must supply rules for building sources it contributes
src/gsl_rng_excl/%.o: ../src/gsl_rng_excl/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


