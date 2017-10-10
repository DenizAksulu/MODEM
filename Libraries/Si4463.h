/*
 * Si4463.h
 *
 *  Created on: 30 Oca 2017
 *      Author: Mehmet Deniz Aksulu
 */

#ifndef LIBRARIES_SI4463_H_
#define LIBRARIES_SI4463_H_

#include "../MODEM.h"
#include "../TX_Config.h"

enum GPIO_MODE {NO_EFFECT, IN_OUT_DISABLED, OUTPUT_LOW, OUTPUT_HIGH, INPUT,
	OSC_32KHZ, BOOT_CLK, DIVIDED_MCU_CLK, CTS, INV_CTS, CMD_OVERLAP, MISO, POR, CALIB_TIM, WAKEUP_TIM,
	UNUSED0, TX_DATA_CLK, RX_DATA_CLK, UNUSED1, TX_DATA, RX_DATA, RX_RAW_DATA, ANT_1_SW, ANT_2_SW, VALID_PREAMBLE,
	INVALID_PREAMBLE, SYNC, RSSI, TRANSMIT_STATE, RECEIVE_STATE, RX_FIFO_FULL, TX_FIFO_EMPTY, VBAT_LOW, RSSI_SYNC,
	HOP, HOP_TABLE};
enum PULL_MODE {DISABLE_PULLUP, ENABLE_PULLUP};
enum DRIVE_MODE {DRV_HIGH, DRV_MEDIUM, DRV_LOW = 3};
enum SI4463_STATE {NO_CHANGE, SLEEP, SPI_ACTIVE, READY1, READY2, TUNE_TX, TUNE_RX, TX, RX};
enum FRR_CTL_MODE {GLOBAL_STATUS = 1, GLOBAL_PENDING = 2, PACKET_HANDLER_STATUS = 3, PACKET_HANDLER_PENDING = 4, MODEM_STATUS = 5,
				   MODEM_PENDING = 6, CHIP_STATUS = 7, CHIP_PENDING = 8, CURRENT_STATE = 9};
//enum MODEM_TYPE {RX, TX};

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
	unsigned char SetGPIO(GPIO_MODE GPIO0, GPIO_MODE GPIO1, SI4463_STATE TYPE);
	unsigned char Temperature(unsigned char* TEMPERATURE);
	unsigned char Temperature(double* TEMPERATURE);
	unsigned char FIFOInfo(unsigned char RESET_RX_FIFO, unsigned char RESET_TX_FIFO, unsigned char* DATA);
	unsigned char GetIntStatus(unsigned char* PARAMETERS, unsigned char* DATA);
	unsigned char GetPHStatus(unsigned char* DATA);
	unsigned char GetModemStatus(unsigned char* DATA);
	unsigned char GetChipStatus(unsigned char* DATA);
	unsigned char StartTX(unsigned char CHANNEL, unsigned char CONDITION, uint16_t LENGTH);
	unsigned char StartTX(uint16_t LENGTH);
	unsigned char StartRX(unsigned char CHANNEL, unsigned char CONDITION, uint16_t LENGTH);
	unsigned char StartRX(uint16_t LENGTH);
	unsigned char StartRX(uint16_t LENGTH, void (*interruptVector)(void));
	unsigned char StartRX_ISR(uint16_t LENGTH, portBASE_TYPE* xHigherPriorityTaskWoken);
	unsigned char GetDeviceState(SI4463_STATE* STATE);
	unsigned char ChangeState(SI4463_STATE STATE);
	unsigned char ChangeState_ISR(SI4463_STATE STATE, portBASE_TYPE* xHigherPriorityTaskWoken);
	unsigned char ReadFRR(unsigned char* DATA);
	unsigned char ReadFRR_ISR(unsigned char* DATA);
	unsigned char WriteTXFIFO(unsigned char* DATA, uint16_t LENGTH);
	unsigned char ReadRXFIFO(unsigned char* DATA, uint16_t LENGTH);
	unsigned char ReadRXFIFO_ISR(unsigned char* DATA, uint16_t LENGTH);
	unsigned char SetFrequency(unsigned int FREQ);
	unsigned char SetTXModem();
	unsigned char SetRXModem();
	unsigned char SetFRR(FRR_CTL_MODE FRR_A, FRR_CTL_MODE FRR_B, FRR_CTL_MODE FRR_C, FRR_CTL_MODE FRR_D);
	unsigned char SetPA(unsigned char PA);
	unsigned char Broadcast(unsigned char* DATA, uint16_t LENGTH);
	unsigned char ClearInterrupts();
	unsigned char ClearInterrupts_ISR(portBASE_TYPE* xHigherPriorityTaskWoken);
	unsigned char SendData(unsigned char* DATA, unsigned int LENGTH);

private:
	unsigned char _cmd(unsigned char* BUFFER, unsigned char CMD_LENGTH, unsigned char RES_LENGTH);
	unsigned char _cmd_ISR(unsigned char* BUFFER, unsigned char CMD_LENGTH, unsigned char RES_LENGTH, portBASE_TYPE* xHigherPriorityTaskWoken);
	xSemaphoreHandle _mutex;
	SPI* _spi;
	DigitalOut* _nsel;
	DigitalOut* _sdn;
	unsigned int _timeout;
};

#endif /* LIBRARIES_SI4463_H_ */
