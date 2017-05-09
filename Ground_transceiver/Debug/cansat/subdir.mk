################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/zzuummaa/cansat/filewriter.cpp \
/home/zzuummaa/cansat/i2c.cpp \
/home/zzuummaa/cansat/image.cpp \
/home/zzuummaa/cansat/pipe.cpp \
/home/zzuummaa/cansat/structs.cpp \
/home/zzuummaa/cansat/telemetry_pipe.cpp 

OBJS += \
./cansat/filewriter.o \
./cansat/i2c.o \
./cansat/image.o \
./cansat/pipe.o \
./cansat/structs.o \
./cansat/telemetry_pipe.o 

CPP_DEPS += \
./cansat/filewriter.d \
./cansat/i2c.d \
./cansat/image.d \
./cansat/pipe.d \
./cansat/structs.d \
./cansat/telemetry_pipe.d 


# Each subdirectory must supply rules for building sources it contributes
cansat/filewriter.o: /home/zzuummaa/cansat/filewriter.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/zzuummaa/cansat -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

cansat/i2c.o: /home/zzuummaa/cansat/i2c.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/zzuummaa/cansat -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

cansat/image.o: /home/zzuummaa/cansat/image.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/zzuummaa/cansat -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

cansat/pipe.o: /home/zzuummaa/cansat/pipe.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/zzuummaa/cansat -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

cansat/structs.o: /home/zzuummaa/cansat/structs.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/zzuummaa/cansat -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

cansat/telemetry_pipe.o: /home/zzuummaa/cansat/telemetry_pipe.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I/home/zzuummaa/cansat -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


