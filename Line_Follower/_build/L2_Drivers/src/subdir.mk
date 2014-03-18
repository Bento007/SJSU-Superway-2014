################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../L2_Drivers/src/i2c2.cpp \
../L2_Drivers/src/lpc_pwm.cpp \
../L2_Drivers/src/nrf_stream.cpp \
../L2_Drivers/src/uart0.cpp \
../L2_Drivers/src/uart2.cpp \
../L2_Drivers/src/uart3.cpp 

C_SRCS += \
../L2_Drivers/src/can.c \
../L2_Drivers/src/eint.c \
../L2_Drivers/src/rtc.c \
../L2_Drivers/src/spi_dma.c \
../L2_Drivers/src/spi_sem.c \
../L2_Drivers/src/ssp1.c 

OBJS += \
./L2_Drivers/src/can.o \
./L2_Drivers/src/eint.o \
./L2_Drivers/src/i2c2.o \
./L2_Drivers/src/lpc_pwm.o \
./L2_Drivers/src/nrf_stream.o \
./L2_Drivers/src/rtc.o \
./L2_Drivers/src/spi_dma.o \
./L2_Drivers/src/spi_sem.o \
./L2_Drivers/src/ssp1.o \
./L2_Drivers/src/uart0.o \
./L2_Drivers/src/uart2.o \
./L2_Drivers/src/uart3.o 

C_DEPS += \
./L2_Drivers/src/can.d \
./L2_Drivers/src/eint.d \
./L2_Drivers/src/rtc.d \
./L2_Drivers/src/spi_dma.d \
./L2_Drivers/src/spi_sem.d \
./L2_Drivers/src/ssp1.d 

CPP_DEPS += \
./L2_Drivers/src/i2c2.d \
./L2_Drivers/src/lpc_pwm.d \
./L2_Drivers/src/nrf_stream.d \
./L2_Drivers/src/uart0.d \
./L2_Drivers/src/uart2.d \
./L2_Drivers/src/uart3.d 


# Each subdirectory must supply rules for building sources it contributes
L2_Drivers/src/%.o: ../L2_Drivers/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\newlib" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L0_LowLevel" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L1_FreeRTOS" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L1_FreeRTOS\include" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L1_FreeRTOS\portable" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L2_Drivers" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L2_Drivers\base" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L3_Utils" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L3_Utils\tlm" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L4_IO" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L4_IO\fat" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L4_IO\wireless" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L5_Application" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

L2_Drivers/src/%.o: ../L2_Drivers/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\newlib" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L0_LowLevel" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L1_FreeRTOS" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L1_FreeRTOS\include" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L1_FreeRTOS\portable" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L2_Drivers" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L2_Drivers\base" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L3_Utils" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L3_Utils\tlm" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L4_IO" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L4_IO\fat" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L4_IO\wireless" -I"C:\Users\Elizabeth\Documents\SJSU_Dev\projects\Line_Follower\L5_Application" -fno-exceptions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


