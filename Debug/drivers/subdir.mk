################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_anatop.c \
../drivers/fsl_anatop_ai.c \
../drivers/fsl_cache.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_csi.c \
../drivers/fsl_dcdc.c \
../drivers/fsl_elcdif.c \
../drivers/fsl_flexspi.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lcdifv2.c \
../drivers/fsl_lpi2c.c \
../drivers/fsl_lpspi.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_mipi_csi2rx.c \
../drivers/fsl_mipi_dsi.c \
../drivers/fsl_pmu.c \
../drivers/fsl_pxp.c \
../drivers/fsl_soc_mipi_csi2rx.c \
../drivers/fsl_soc_src.c \
../drivers/fsl_usdhc.c 

OBJS += \
./drivers/fsl_anatop.o \
./drivers/fsl_anatop_ai.o \
./drivers/fsl_cache.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_csi.o \
./drivers/fsl_dcdc.o \
./drivers/fsl_elcdif.o \
./drivers/fsl_flexspi.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lcdifv2.o \
./drivers/fsl_lpi2c.o \
./drivers/fsl_lpspi.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_mipi_csi2rx.o \
./drivers/fsl_mipi_dsi.o \
./drivers/fsl_pmu.o \
./drivers/fsl_pxp.o \
./drivers/fsl_soc_mipi_csi2rx.o \
./drivers/fsl_soc_src.o \
./drivers/fsl_usdhc.o 

C_DEPS += \
./drivers/fsl_anatop.d \
./drivers/fsl_anatop_ai.d \
./drivers/fsl_cache.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_csi.d \
./drivers/fsl_dcdc.d \
./drivers/fsl_elcdif.d \
./drivers/fsl_flexspi.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lcdifv2.d \
./drivers/fsl_lpi2c.d \
./drivers/fsl_lpspi.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_mipi_csi2rx.d \
./drivers/fsl_mipi_dsi.d \
./drivers/fsl_pmu.d \
./drivers/fsl_pxp.d \
./drivers/fsl_soc_mipi_csi2rx.d \
./drivers/fsl_soc_src.d \
./drivers/fsl_usdhc.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1176DVMAA -DCPU_MIMXRT1176DVMAA_cm7 -DXIP_BOOT_HEADER_DCD_ENABLE=1 -DUSE_SDRAM -DDATA_SECTION_IS_CACHEABLE=1 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSDK_I2C_BASED_COMPONENT_USED=1 -DSERIAL_PORT_TYPE_UART=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\board" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\spiFash" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\sdmmc\host" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\sdmmc\inc" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\fatfs\source\fsl_sd_disk" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\fatfs\source" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\libjpeg\inc" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\fatfs\source" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\component\osa" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\sdmmc\osa" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\source" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\video" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\drivers" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\device" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\CMSIS" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\utilities" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\component\serial_manager" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\component\lists" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\component\uart" -I"C:\Users\Farid Mabrouk\Documents\HDtrap_GIT\TrapHD_CSI_MIPI\TrapHD_CSI_MIPI\xip" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


