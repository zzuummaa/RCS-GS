################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/zzuummaa/cansat/trs/protocol.cpp \
/home/zzuummaa/cansat/trs/transceiver.cpp 

OBJS += \
./cansat/trs/protocol.o \
./cansat/trs/transceiver.o 

CPP_DEPS += \
./cansat/trs/protocol.d \
./cansat/trs/transceiver.d 


# Each subdirectory must supply rules for building sources it contributes
cansat/trs/protocol.o: /home/zzuummaa/cansat/trs/protocol.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/zzuummaa/cansat -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

cansat/trs/transceiver.o: /home/zzuummaa/cansat/trs/transceiver.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/zzuummaa/cansat -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


