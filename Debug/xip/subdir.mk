################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../xip/evkmimxrt1170_flexspi_nor_config.c \
../xip/fsl_flexspi_nor_boot.c 

OBJS += \
./xip/evkmimxrt1170_flexspi_nor_config.o \
./xip/fsl_flexspi_nor_boot.o 

C_DEPS += \
./xip/evkmimxrt1170_flexspi_nor_config.d \
./xip/fsl_flexspi_nor_boot.d 


# Each subdirectory must supply rules for building sources it contributes
xip/%.o: ../xip/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1176DVMAA -DCPU_MIMXRT1176DVMAA_cm7 -DXIP_BOOT_HEADER_DCD_ENABLE=1 -DUSE_SDRAM -DDATA_SECTION_IS_CACHEABLE=1 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSDK_I2C_BASED_COMPONENT_USED=1 -DSERIAL_PORT_TYPE_UART=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\board" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\spiFlash" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\sdmmc\host" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\sdmmc\inc" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\fatfs\source\fsl_sd_disk" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\fatfs\source" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\libjpeg\inc" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\fatfs\source" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\component\osa" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\sdmmc\osa" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\source" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\video" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\drivers" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\device" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\CMSIS" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\utilities" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\component\serial_manager" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\component\lists" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\component\uart" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\xip" -O2 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


