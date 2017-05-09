################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/zzuummaa/cansat/shm/redis.cpp 

OBJS += \
./cansat/shm/redis.o 

CPP_DEPS += \
./cansat/shm/redis.d 


# Each subdirectory must supply rules for building sources it contributes
cansat/shm/redis.o: /home/zzuummaa/cansat/shm/redis.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/zzuummaa/cansat -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


