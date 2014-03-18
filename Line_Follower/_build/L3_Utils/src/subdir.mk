################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../L3_Utils/src/command_handler.cpp \
../L3_Utils/src/file_logger.cpp \
../L3_Utils/src/freertos_timer.cpp \
../L3_Utils/src/logger.cpp \
../L3_Utils/src/scheduler_task.cpp \
../L3_Utils/src/str.cpp 

C_SRCS += \
../L3_Utils/src/c_list.c \
../L3_Utils/src/rtc_alarm.c \
../L3_Utils/src/utilities.c 

OBJS += \
./L3_Utils/src/c_list.o \
./L3_Utils/src/command_handler.o \
./L3_Utils/src/file_logger.o \
./L3_Utils/src/freertos_timer.o \
./L3_Utils/src/logger.o \
./L3_Utils/src/rtc_alarm.o \
./L3_Utils/src/scheduler_task.o \
./L3_Utils/src/str.o \
./L3_Utils/src/utilities.o 

C_DEPS += \
./L3_Utils/src/c_list.d \
./L3_Utils/src/rtc_alarm.d \
./L3_Utils/src/utilities.d 

CPP_DEPS += \
./L3_Utils/src/command_handler.d \
./L3_Utils/src/file_logger.d \
./L3_Utils/src/freertos_timer.d \
./L3_Utils/src/logger.d \
./L3_Utils/src/scheduler_task.d \
./L3_Utils/src/str.d 


# Each subdirectory must supply rules for building sources it contributes
L3_Utils/src/%.o: ../L3_Utils/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\newlib" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L0_LowLevel" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L1_FreeRTOS" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L1_FreeRTOS\include" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L1_FreeRTOS\portable" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L2_Drivers" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L2_Drivers\base" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L3_Utils" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L3_Utils\tlm" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L4_IO" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L4_IO\fat" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L4_IO\wireless" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L5_Application" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

L3_Utils/src/%.o: ../L3_Utils/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\newlib" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L0_LowLevel" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L1_FreeRTOS" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L1_FreeRTOS\include" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L1_FreeRTOS\portable" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L2_Drivers" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L2_Drivers\base" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L3_Utils" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L3_Utils\tlm" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L4_IO" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L4_IO\fat" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L4_IO\wireless" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L5_Application" -fno-exceptions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


