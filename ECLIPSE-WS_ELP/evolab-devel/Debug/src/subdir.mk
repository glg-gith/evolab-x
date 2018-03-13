################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ex1_mass_action.cpp \
../src/ex2_percolation.cpp \
../src/ex3_chain_binomial.cpp \
../src/ex4_dynamic_net.cpp \
../src/ex5_diff_eq.cpp \
../src/ex6_network_diff_eq.cpp \
../src/ex7_seir_percolation.cpp 

OBJS += \
./src/ex1_mass_action.o \
./src/ex2_percolation.o \
./src/ex3_chain_binomial.o \
./src/ex4_dynamic_net.o \
./src/ex5_diff_eq.o \
./src/ex6_network_diff_eq.o \
./src/ex7_seir_percolation.o 

CPP_DEPS += \
./src/ex1_mass_action.d \
./src/ex2_percolation.d \
./src/ex3_chain_binomial.d \
./src/ex4_dynamic_net.d \
./src/ex5_diff_eq.d \
./src/ex6_network_diff_eq.d \
./src/ex7_seir_percolation.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/golgauth/Documents/02_DEV/ECLIPSE-WS_ELP/evolab-devel/inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


