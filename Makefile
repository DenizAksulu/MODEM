GCC_BIN = 
PROJECT = MODEM
OBJECTS = ./Libraries/PC.o ./PCHandlingTask.o ./CommandHandlingTask.o ./TXTask.o ./RXTask.o ./Libraries/Si4463.o ./MODEM.o  ./FreeRTOS/Source/event_groups.o ./FreeRTOS/Source/croutine.o ./FreeRTOS/Source/list.o ./FreeRTOS/Source/queue.o ./FreeRTOS/Source/tasks.o ./FreeRTOS/Source/timers.o ./FreeRTOS/Source/portable/RVDS/ARM_CM3/port.o ./FreeRTOS/Source/portable/MemMang/heap_1.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/TARGET_STM32F407VG/TOOLCHAIN_GCC_ARM/startup_stm32f407xx.o ./mbed-src/common/assert.o ./mbed-src/common/board.o ./mbed-src/common/error.o ./mbed-src/common/gpio.o ./mbed-src/common/lp_ticker_api.o ./mbed-src/common/mbed_interface.o ./mbed-src/common/pinmap_common.o ./mbed-src/common/rtc_time.o ./mbed-src/common/semihost_api.o ./mbed-src/common/ticker_api.o ./mbed-src/common/us_ticker_api.o ./mbed-src/common/wait_api.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_adc.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_adc_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_can.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_cec.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_cortex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_crc.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_cryp.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_cryp_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_dac.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_dac_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_dcmi.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_dcmi_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_dma.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_dma2d.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_dma_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_eth.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_flash.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_flash_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_flash_ramfunc.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_fmpi2c.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_fmpi2c_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_gpio.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_hash.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_hash_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_hcd.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_i2c.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_i2c_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_i2s.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_i2s_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_irda.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_iwdg.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_ltdc.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_nand.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_nor.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_pccard.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_pcd.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_pcd_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_pwr.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_pwr_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_qspi.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_rcc.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_rcc_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_rng.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_rtc.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_rtc_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_sai.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_sai_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_sd.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_sdram.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_smartcard.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_spdifrx.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_spi.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_sram.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_tim.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_tim_ex.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_uart.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_usart.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_hal_wwdg.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_ll_fmc.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_ll_fsmc.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_ll_sdmmc.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/stm32f4xx_ll_usb.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/TARGET_STM32F407VG/cmsis_nvic.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/TARGET_STM32F407VG/hal_tick.o ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/TARGET_STM32F407VG/system_stm32f4xx.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/analogin_api.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/analogout_api.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/gpio_api.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/gpio_irq_api.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/i2c_api.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/mbed_overrides.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/pinmap.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/port_api.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/pwmout_api.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/rtc_api.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/serial_api.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/sleep.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/spi_api.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/us_ticker.o ./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/TARGET_ARCH_MAX/PeripheralPins.o main.o ./mbed-src/common/BusIn.o ./mbed-src/common/BusInOut.o ./mbed-src/common/BusOut.o ./mbed-src/common/CAN.o ./mbed-src/common/CallChain.o ./mbed-src/common/Ethernet.o ./mbed-src/common/FileBase.o ./mbed-src/common/FileLike.o ./mbed-src/common/FilePath.o ./mbed-src/common/FileSystemLike.o ./mbed-src/common/I2C.o ./mbed-src/common/I2CSlave.o ./mbed-src/common/InterruptIn.o ./mbed-src/common/InterruptManager.o ./mbed-src/common/LocalFileSystem.o ./mbed-src/common/RawSerial.o ./mbed-src/common/SPI.o ./mbed-src/common/SPISlave.o ./mbed-src/common/Serial.o ./mbed-src/common/SerialBase.o ./mbed-src/common/Stream.o ./mbed-src/common/Ticker.o ./mbed-src/common/Timeout.o ./mbed-src/common/Timer.o ./mbed-src/common/TimerEvent.o ./mbed-src/common/retarget.o ./SDFileSystem/SDCRC.o ./SDFileSystem/SDFileSystem.o ./SDFileSystem/FATFileSystem/FATDirHandle.o ./SDFileSystem/FATFileSystem/FATFileHandle.o ./SDFileSystem/FATFileSystem/FATFileSystem.o ./SDFileSystem/FATFileSystem/ChaN/ccsbcs.o ./SDFileSystem/FATFileSystem/ChaN/diskio.o ./SDFileSystem/FATFileSystem/ChaN/ff.o 
SYS_OBJECTS = 
INCLUDE_PATHS = -I./Libraries -I./FreeRTOS -I./FreeRTOS/Source -I./FreeRTOS/Source/include -I./FreeRTOS/Source/portable -I./FreeRTOS/Source/portable/RVDS -I./FreeRTOS/Source/portable/RVDS/ARM_CM3 -I./FreeRTOS/Source/portable/MemMang -I./ -I./mbed-src -I./mbed-src/api -I./mbed-src/common -I./mbed-src/hal -I./mbed-src/targets -I./mbed-src/targets/cmsis -I./mbed-src/targets/cmsis/TARGET_STM -I./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4 -I./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/TARGET_STM32F407VG -I./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/TARGET_STM32F407VG/TOOLCHAIN_GCC_ARM -I./mbed-src/targets/cmsis/TOOLCHAIN_GCC -I./mbed-src/targets/hal -I./mbed-src/targets/hal/TARGET_STM -I./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4 -I./mbed-src/targets/hal/TARGET_STM/TARGET_STM32F4/TARGET_ARCH_MAX -I./SDFileSystem -I./SDFileSystem/FATFileSystem -I./SDFileSystem/FATFileSystem/ChaN 
LIBRARY_PATHS = 
LIBRARIES = 
LINKER_SCRIPT = ./mbed-src/targets/cmsis/TARGET_STM/TARGET_STM32F4/TARGET_STM32F407VG/TOOLCHAIN_GCC_ARM/STM32F407XG.ld
############################################################################### 
AS      = $(GCC_BIN)arm-none-eabi-as
CC      = $(GCC_BIN)arm-none-eabi-gcc
CPP     = $(GCC_BIN)arm-none-eabi-g++
LD      = $(GCC_BIN)arm-none-eabi-gcc
OBJCOPY = $(GCC_BIN)arm-none-eabi-objcopy
OBJDUMP = $(GCC_BIN)arm-none-eabi-objdump
SIZE    = $(GCC_BIN)arm-none-eabi-size 

ifeq ($(HARDFP),1)
	FLOAT_ABI = hard
else
	FLOAT_ABI = softfp
endif

CPU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=$(FLOAT_ABI) 
CC_FLAGS = $(CPU) -c -g -fno-common -fmessage-length=0 -Wall -Wextra -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer -MMD -MP
CC_SYMBOLS = -DTARGET_RTOS_M4_M7 -DTARGET_ARCH_MAX -DTOOLCHAIN_GCC_ARM -DTOOLCHAIN_GCC -D__CORTEX_M4 -DTARGET_CORTEX_M -DLSI_VALUE=32000 -D__FPU_PRESENT=1 -DTARGET_FF_ARDUINO -D__MBED__=1 -DTARGET_M4 -DTARGET_STM -DTARGET_STM32F407 -DTARGET_STM32F4 -DTARGET_STM32F407VG -DMBED_BUILD_TIMESTAMP=1454608702.62 -DARM_MATH_CM4 

LD_FLAGS = $(CPU) -Wl,--gc-sections --specs=nano.specs -u _printf_float -u _scanf_float -Wl,--wrap,main -Wl,-Map=$(PROJECT).map,--cref
LD_SYS_LIBS = -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys


ifeq ($(DEBUG), 1)
	CC_FLAGS += -DDEBUG -Og
else
	CC_FLAGS += -DNDEBUG -Os
endif

.PHONY: all clean lst size

all: $(PROJECT).bin $(PROJECT).hex size


clean:
	rm -f $(PROJECT).bin $(PROJECT).elf $(PROJECT).hex $(PROJECT).map $(PROJECT).lst $(OBJECTS) $(DEPS)


.asm.o:
	$(CC) $(CPU) -c -x assembler-with-cpp -o $@ $<
.s.o:
	$(CC) $(CPU) -c -x assembler-with-cpp -o $@ $<
.S.o:
	$(CC) $(CPU) -c -x assembler-with-cpp -o $@ $<

.c.o:
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu99   $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu++98 -fno-rtti $(INCLUDE_PATHS) -o $@ $<



$(PROJECT).elf: $(OBJECTS) $(SYS_OBJECTS)
	$(LD) $(LD_FLAGS) -T$(LINKER_SCRIPT) $(LIBRARY_PATHS) -o $@ $^ $(LIBRARIES) $(LD_SYS_LIBS) $(LIBRARIES) $(LD_SYS_LIBS)


$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

$(PROJECT).hex: $(PROJECT).elf
	@$(OBJCOPY) -O ihex $< $@

$(PROJECT).lst: $(PROJECT).elf
	@$(OBJDUMP) -Sdh $< > $@

lst: $(PROJECT).lst

size: $(PROJECT).elf
	$(SIZE) $(PROJECT).elf

DEPS = $(OBJECTS:.o=.d) $(SYS_OBJECTS:.o=.d)
-include $(DEPS)

