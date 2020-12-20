################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sdmmc/common/fsl_sdmmc_common.c 

OBJS += \
./sdmmc/common/fsl_sdmmc_common.o 

C_DEPS += \
./sdmmc/common/fsl_sdmmc_common.d 


# Each subdirectory must supply rules for building sources it contributes
sdmmc/common/%.o: ../sdmmc/common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1176DVMAA -DCPU_MIMXRT1176DVMAA_cm7 -DXIP_BOOT_HEADER_DCD_ENABLE=1 -DUSE_SDRAM -DDATA_SECTION_IS_CACHEABLE=1 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSDK_I2C_BASED_COMPONENT_USED=1 -DSERIAL_PORT_TYPE_UART=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\board" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\fatfs\source\fsl_sd_disk" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\fatfs\source" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\libjpeg\inc" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\fatfs\source" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\component\osa" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\sdmmc\osa" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\sdmmc\common" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\sdmmc\sd" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\source" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\video" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\drivers" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\device" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\CMSIS" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\utilities" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\component\serial_manager" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\component\lists" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\component\uart" -I"C:\Users\Farid Mabrouk\Documents\NXPprojects\TrapHD_csi_mipi_rgb_cm7\xip" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


