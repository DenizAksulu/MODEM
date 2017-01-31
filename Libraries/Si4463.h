/*
 * Si4463.h
 *
 *  Created on: 30 Oca 2017
 *      Author: Mehmet Deniz Aksulu
 */

#ifndef LIBRARIES_SI4463_H_
#define LIBRARIES_SI4463_H_

#include "../MODEM.h"

enum GPIO_MODE {NO_EFFECT, IN_OUT_DISABLED, OUTPUT_LOW, OUTPUT_HIGH, INPUT,
	OSC_32KHZ, BOOT_CLK, DIVIDED_MCU_CLK, CTS, INV_CTS, CMD_OVERLAP, MISO, POR, CALIB_TIM, WAKEUP_TIM,
	UNUSED0, TX_DATA_CLK, RX_DATA_CLK, UNUSED1, TX_DATA, RX_DATA, RX_RAW_DATA, ANT_1_SW, ANT_2_SW, VALID_PREAMBLE,
	INVALID_PREAMBLE, SYNC, RSSI, TRANSMIT_STATE, RECEIVE_STATE, RX_FIFO_FULL, TX_FIFO_EMPTY, VBAT_LOW, RSSI_SYNC,
	HOP, HOP_TABLE};
enum PULL_MODE {DISABLE_PULLUP, ENABLE_PULLUP};
enum DRIVE_MODE {DRV_HIGH, DRV_MEDIUM, DRV_LOW = 3};

class Si4463
{
public:
	Si4463(SPI* spi, DigitalOut* nsel, DigitalOut* sdn);
	unsigned char PowerUp();
	unsigned char NOP();
	unsigned char PartInfo(unsigned char* INFO);
	unsigned char FuncInfo(unsigned char* INFO);
	unsigned char SetProperty(unsigned char GROUP, unsigned char NUM_PROPS, unsigned char START_PROP, unsigned char* DATA);
	unsigned char GetProperty(unsigned char GROUP, unsigned char NUM_PROPS, unsigned char START_PROP, unsigned char* DATA);
	unsigned char SetGPIO(PULL_MODE* pull_mode, GPIO_MODE* gpio_mode, DRIVE_MODE drive_mode);
	unsigned char Temperature(unsigned char* TEMPERATURE);
	unsigned char Temperature(double* TEMPERATURE);
	unsigned char FIFOInfo();//TODO
	unsigned char PacketInfo();//TODO

private:
	unsigned char _cmd(unsigned char* BUFFER, unsigned char CMD_LENGTH, unsigned char RES_LENGTH);
	xSemaphoreHandle _mutex;
	SPI* _spi;
	DigitalOut* _nsel;
	DigitalOut* _sdn;
	unsigned int _timeout;
};

#endif /* LIBRARIES_SI4463_H_ */
