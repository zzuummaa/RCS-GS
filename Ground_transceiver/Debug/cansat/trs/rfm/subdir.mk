################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/zzuummaa/cansat/trs/rfm/rfm22b.cpp \
/home/zzuummaa/cansat/trs/rfm/spi.cpp 

OBJS += \
./cansat/trs/rfm/rfm22b.o \
./cansat/trs/rfm/spi.o 

CPP_DEPS += \
./cansat/trs/rfm/rfm22b.d \
./cansat/trs/rfm/spi.d 


# Each subdirectory must supply rules for building sources it contributes
cansat/trs/rfm/rfm22b.o: /home/zzuummaa/cansat/trs/rfm/rfm22b.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/zzuummaa/cansat -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

cansat/trs/rfm/spi.o: /home/zzuummaa/cansat/trs/rfm/spi.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/zzuummaa/cansat -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


