################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../L5_Application/main.cpp 

OBJS += \
./L5_Application/main.o 

CPP_DEPS += \
./L5_Application/main.d 


# Each subdirectory must supply rules for building sources it contributes
L5_Application/%.o: ../L5_Application/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"C:\Users\Elizabeth\git\SJSU-Superway-2014\Line_Follower" -I"C:\Users\Elizabeth\git\SJSU-Superway-2014\Line_Follower\newlib" -I"C:\Users\Elizabeth\git\SJSU-Superway-2014\Line_Follower\L0_LowLevel" -I"C:\Users\Elizabeth\git\SJSU-Superway-2014\Line_Follower\L1_FreeRTOS" -I"C:\Users\Elizabeth\git\SJSU-Superway-2014\Line_Follower\L1_FreeRTOS\include" -I"C:\Users\Elizabeth\git\SJSU-Superway-2014\Line_Follower\L1_FreeRTOS\portable" -I"C:\Users\Elizabeth\git\SJSU-Superway-2014\Line_Follower\L2_Drivers" -I"C:\Users\Elizabeth\git\SJSU-Superway-2014\Line_Follower\L2_Drivers\base" -I"C:\Users\Elizabeth\git\SJSU-Superway-2014\Line_Follower\L3_Utils" -I"C:\Users\Elizabeth\git\SJSU-Superway-2014\Line_Follower\L3_Utils\tlm" -I"C:\Users\Elizabeth\git\SJSU-Superway-2014\Line_Follower\L4_IO" -I"C:\Users\Elizabeth\git\SJSU-Superway-2014\Line_Follower\L4_IO\fat" -I"C:\Users\Elizabeth\git\SJSU-Superway-2014\Line_Follower\L4_IO\wireless" -I"C:\Users\Elizabeth\git\SJSU-Superway-2014\Line_Follower\L5_Application" -fno-exceptions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


