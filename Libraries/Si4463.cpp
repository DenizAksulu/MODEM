/*
 * Si4463.cpp
 *
 *  Created on: 30 Oca 2017
 *      Author: Mehmet Deniz Aksulu
 */

#include "Si4463.h"

using namespace MODEM;

InterruptIn* SyncDetectInterrupt;

Si4463::Si4463(SPI* spi, DigitalOut* nsel, DigitalOut* sdn)
{
	_spi = spi;							// SPI
	_nsel = nsel;						// NSEL pin
	_sdn = sdn;							// SDN pin
	_sdn->write(1);						// SDN is LOW
	_nsel->write(1);					// NSEL is HIGH
	_mutex = xSemaphoreCreateMutex();	// Create MUTEX
	_timeout = 10000;					// Command timeout in us
	_spi->frequency(10000000);			// 10 MHz
	_spi->format(8);					// 1 | 0
}

unsigned char Si4463::PowerUp()
{
	if(xSemaphoreTake(_mutex, portMAX_DELAY) == pdTRUE)
	{
		_sdn->write(1);
		vTaskDelay(1);						// Delay 1 ms
		_sdn->write(0);
		vTaskDelay(15);						// Delay 15 ms
		xSemaphoreGive(_mutex);
	}

	// CMD = 0x02
	// BOOT_OPTION = 0x01 -> Boot main application image
	// XTAL option = 0x00 -> XTAL used
	// Crystal freq. = 30000000 (0x01C9C380)
	unsigned char BUFFER[7] = {0x02, 0x01, 0x00, 0x01, 0xC9, 0xC3, 0x80};
	return _cmd(BUFFER, 7, 0);				// Send power up command
}

unsigned char Si4463::NOP()
{
	unsigned char NOP_COMMAND = 0x00;
	return _cmd(&NOP_COMMAND, 1, 0);
}

unsigned char Si4463::PartInfo(unsigned char* INFO)
{
	INFO[0] = 0x01;
	return _cmd(INFO, 1, 8);
}

unsigned char Si4463::FuncInfo(unsigned char* INFO)
{
	INFO[0] = 0x10;
	return _cmd(INFO, 1, 6);
}

unsigned char Si4463::SetProperty(unsigned char GROUP, unsigned char NUM_PROPS, unsigned char START_PROP, unsigned char* DATA)
{
	unsigned char BUFFER[4 + NUM_PROPS];
	BUFFER[0] = 0x11;
	BUFFER[1] = GROUP;
	BUFFER[2] = NUM_PROPS;
	BUFFER[3] = START_PROP;
	for(int i = 0; i < NUM_PROPS; i++)
	{
		BUFFER[4 + i] = DATA[i];
	}
	return _cmd(BUFFER, 4 + NUM_PROPS, 0);
}

unsigned char Si4463::GetProperty(unsigned char GROUP, unsigned char NUM_PROPS, unsigned char START_PROP, unsigned char* DATA)
{
	unsigned char BUFFER[NUM_PROPS];
	BUFFER[0] = 0x12;
	BUFFER[1] = GROUP;
	BUFFER[2] = NUM_PROPS;
	BUFFER[3] = START_PROP;
	unsigned char result = _cmd(BUFFER, 4, NUM_PROPS);
	for(int i = 0; i < NUM_PROPS; i++)
	{
		DATA[i] = BUFFER[i];
	}
	return result;
}

unsigned char Si4463::SetGPIO(PULL_MODE* pull_mode, GPIO_MODE* gpio_mode, DRIVE_MODE drive_mode = DRV_HIGH)
{
	unsigned char BUFFER[8] = {0};
	unsigned char buffer_index = 0;
	BUFFER[buffer_index++] = 0x13;
	if(pull_mode[buffer_index - 1] == DISABLE_PULLUP)
		BUFFER[buffer_index] = gpio_mode[buffer_index - 1];
	else
		BUFFER[buffer_index] = gpio_mode[buffer_index - 1] | (0x01 << 6);
	buffer_index++;
	if(pull_mode[buffer_index - 1] == DISABLE_PULLUP)
			BUFFER[buffer_index] = gpio_mode[buffer_index - 1];
	else
		BUFFER[buffer_index] = gpio_mode[buffer_index - 1] | (0x01 << 6);
	buffer_index++;
	if(pull_mode[buffer_index - 1] == DISABLE_PULLUP)
			BUFFER[buffer_index] = gpio_mode[buffer_index - 1];
	else
	BUFFER[buffer_index] = gpio_mode[buffer_index - 1] | (0x01 << 6);
	buffer_index++;
	if(pull_mode[buffer_index - 1] == DISABLE_PULLUP)
	BUFFER[buffer_index] = gpio_mode[buffer_index - 1];
	else
	BUFFER[buffer_index] = gpio_mode[buffer_index - 1] | (0x01 << 6);
	buffer_index++;
	if(pull_mode[buffer_index - 1] == DISABLE_PULLUP)
	BUFFER[buffer_index] = gpio_mode[buffer_index - 1];
	else
	BUFFER[buffer_index] = gpio_mode[buffer_index - 1] | (0x01 << 6);
	buffer_index++;
	if(pull_mode[buffer_index - 1] == DISABLE_PULLUP)
		BUFFER[buffer_index] = gpio_mode[buffer_index - 1];
	else
		BUFFER[buffer_index] = gpio_mode[buffer_index - 1] | (0x01 << 6);
	buffer_index++;
	BUFFER[buffer_index++] = drive_mode << 5;
	return _cmd(BUFFER, 8, 7);
}

unsigned char Si4463::SetGPIO(GPIO_MODE GPIO0, GPIO_MODE GPIO1, SI4463_STATE TYPE)
{
	//GPIO3 - VC2 - HIGH
	//GPIO2 - VC1 - LOW   => OUT1 (TX)
	unsigned char Buffer[8] = {0x13, GPIO0, GPIO1, OUTPUT_LOW, OUTPUT_HIGH, 0x00, 0x00, 0x00}; // TX Case
	/*RX Case*/
	if(TYPE == RX)
	{
		Buffer[3] = OUTPUT_HIGH;
		Buffer[4] = OUTPUT_LOW;
	}
	if(!_cmd(Buffer, 8, 0)) return 0;
	return 1;
}

unsigned char Si4463::Temperature(unsigned char* TEMPERATURE)
{
	unsigned char result = 0;
	unsigned char BUFFER[8] = {0};
	BUFFER[0] = 0x14;
	BUFFER[1] = 0x10;
	result = _cmd(BUFFER, 2, 8);
	for(int i = 0; i < 4; i++)
		TEMPERATURE[i] = BUFFER[4 + i];
	return result;
}

unsigned char Si4463::Temperature(double* TEMPERATURE)
{
	unsigned char result = 0;
	unsigned char BUFFER[8] = {0};
	BUFFER[0] = 0x14;
	BUFFER[1] = 0x10;
	result = _cmd(BUFFER, 2, 8);
	*TEMPERATURE = ((800 + (double)BUFFER[6]) / 4096) * ((double)BUFFER[5] + (double)BUFFER[4] * 256) - ((double)BUFFER[7] / 2 + 256);
	return result;
}

unsigned char Si4463::FIFOInfo(unsigned char RESET_RX_FIFO, unsigned char RESET_TX_FIFO, unsigned char* DATA)
{
	unsigned char BUFFER[2] = {0};
	BUFFER[0] = 0x15;
	BUFFER[1] = 2 * RESET_RX_FIFO + RESET_TX_FIFO;
	if(!_cmd(BUFFER, 2, 2))
		return 0;
	DATA[0] = BUFFER[0];
	DATA[1] = BUFFER[1];
	return 1;
}

unsigned char Si4463::GetIntStatus(unsigned char* PARAMETERS, unsigned char* DATA)
{
	unsigned char BUFFER[8] = {0};
	BUFFER[0] = 0x20;
	BUFFER[1] = PARAMETERS[0];
	BUFFER[2] = PARAMETERS[1];
	BUFFER[3] = PARAMETERS[2];
	if(!_cmd(BUFFER, 4, 8))
		return 0;
	for(int i = 0; i < 8; i++)
		DATA[i] = BUFFER[i];
	return 1;
}

unsigned char Si4463::GetPHStatus(unsigned char* DATA)
{
	unsigned char BUFFER[2] = {0};
	BUFFER[0] = 0x21;
	if(!_cmd(BUFFER, 1, 2))
		return 0;
	DATA[0] = BUFFER[0];
	DATA[1] = BUFFER[1];
	return 1;
}

unsigned char Si4463::GetModemStatus(unsigned char* DATA)
{
	unsigned char BUFFER[8] = {0};
	BUFFER[0] = 0x22;
	if(!_cmd(BUFFER, 1, 8))
		return 0;
	for(int i = 0; i < 8; i++)
		DATA[i] = BUFFER[i];
	return 1;
}

unsigned char Si4463::GetChipStatus(unsigned char* DATA)
{
	unsigned char BUFFER[3] = {0};
	BUFFER[0] = 0x23;
	if(!_cmd(BUFFER, 1, 2))
		return 0;
	for(int i = 0; i < 3; i++)
		DATA[i] = BUFFER[i];
	return 1;
}

unsigned char Si4463::StartTX(unsigned char CHANNEL, unsigned char CONDITION, uint16_t LENGTH)
{
	unsigned char BUFFER[5] = {0};
	BUFFER[0] = 0x31;
	BUFFER[1] = CHANNEL;
	BUFFER[2] = CONDITION;
	unsigned char LENGTH_CHAR[2] = {0};
	*(uint16_t*)LENGTH_CHAR = LENGTH;
	BUFFER[3] = LENGTH_CHAR[1];
	BUFFER[4] = LENGTH_CHAR[0];
	if(!_cmd(BUFFER, 5, 0))
		return 0;
	return 1;
}
unsigned char Si4463::StartTX(uint16_t LENGTH)
{
	unsigned char BUFFER[5] = {0};
	BUFFER[0] = 0x31;
	BUFFER[1] = 0;
	BUFFER[2] = 0;
	unsigned char LENGTH_CHAR[2] = {0};
	*(uint16_t*)LENGTH_CHAR = LENGTH;
	BUFFER[3] = LENGTH_CHAR[1];
	BUFFER[4] = LENGTH_CHAR[0];
	if(!_cmd(BUFFER, 5, 0))
		return 0;
	return 1;
}

unsigned char Si4463::StartRX(unsigned char CHANNEL, unsigned char CONDITION, uint16_t LENGTH)
{
	unsigned char BUFFER[8] = {0};
	BUFFER[0] = 0x32;
	BUFFER[1] = CHANNEL;
	BUFFER[2] = CONDITION;
	unsigned char LENGTH_CHAR[2] = {0};
	*(uint16_t*)LENGTH_CHAR = LENGTH;
	BUFFER[3] = LENGTH_CHAR[1];
	BUFFER[4] = LENGTH_CHAR[0];
	BUFFER[5] = 0;
	BUFFER[6] = 0;
	BUFFER[7] = 0;
	if(!_cmd(BUFFER, 8, 0))
		return 0;
	return 1;
}
unsigned char Si4463::StartRX(uint16_t LENGTH)
{
	unsigned char BUFFER[8] = {0};
	BUFFER[0] = 0x32;
	BUFFER[1] = 0;
	BUFFER[2] = 0;
	unsigned char LENGTH_CHAR[2] = {0};
	*(uint16_t*)LENGTH_CHAR = LENGTH;
	BUFFER[3] = LENGTH_CHAR[1];
	BUFFER[4] = LENGTH_CHAR[0];
	BUFFER[5] = 0;
	BUFFER[6] = 0;
	BUFFER[7] = 0;
	if(!_cmd(BUFFER, 8, 0))
		return 0;
	return 1;
}

unsigned char Si4463::StartRX(uint16_t LENGTH, void (*interruptVector)(void))
{
	ClearInterrupts();
	SyncDetectInterrupt = new InterruptIn(PD_9);
	SyncDetectInterrupt->rise(interruptVector);
	unsigned char BUFFER[8] = {0};
	BUFFER[0] = 0x32;
	BUFFER[1] = 0;
	BUFFER[2] = 0;
	unsigned char LENGTH_CHAR[2] = {0};
	*(uint16_t*)LENGTH_CHAR = LENGTH;
	BUFFER[3] = LENGTH_CHAR[1];
	BUFFER[4] = LENGTH_CHAR[0];
	BUFFER[5] = 0;
	BUFFER[6] = 0;
	BUFFER[7] = 0;
	if(!_cmd(BUFFER, 8, 0))
		return 0;
	return 1;
}


unsigned char Si4463::StartRX_ISR(uint16_t LENGTH, portBASE_TYPE* xHigherPriorityTaskWoken)
{
	unsigned char BUFFER[8] = {0};
	BUFFER[0] = 0x32;
	BUFFER[1] = 0;
	BUFFER[2] = 0;
	unsigned char LENGTH_CHAR[2] = {0};
	*(uint16_t*)LENGTH_CHAR = LENGTH;
	BUFFER[3] = LENGTH_CHAR[1];
	BUFFER[4] = LENGTH_CHAR[0];
	BUFFER[5] = 0;
	BUFFER[6] = 0;
	BUFFER[7] = 0;
	if(!_cmd_ISR(BUFFER, 8, 0, xHigherPriorityTaskWoken))
		return 0;
	return 1;
}

unsigned char Si4463::GetDeviceState(SI4463_STATE* STATE)
{
	unsigned char BUFFER[2] = {0};
	BUFFER[0] = 0x33;
	if(!_cmd(BUFFER, 1, 2))
		return 0;
	*STATE = (SI4463_STATE)(BUFFER[0] & 0x07);
	return 1;
}

unsigned char Si4463::ChangeState(SI4463_STATE STATE)
{
	unsigned char BUFFER[2] = {0};
	BUFFER[0] = 0x34;
	BUFFER[1] = STATE;
	if(!_cmd(BUFFER, 2, 0))
		return 0;
	return 1;
}

unsigned char Si4463::ChangeState_ISR(SI4463_STATE STATE, portBASE_TYPE* xHigherPriorityTaskWoken)
{
	unsigned char BUFFER[2] = {0};
	BUFFER[0] = 0x34;
	BUFFER[1] = STATE;
	if(!_cmd_ISR(BUFFER, 2, 0, xHigherPriorityTaskWoken))
		return 0;
	return 1;
}

unsigned char Si4463::ReadFRR(unsigned char* DATA)
{
	if(xSemaphoreTake(_mutex, portMAX_DELAY) == pdTRUE)
	{
		_nsel->write(0); 							// NSEL is LOW
		_spi->write(0x50);
		for(unsigned char i = 0; i < 4; i++)
		{
			DATA[i] = _spi->write(0xFF);
		}
		_nsel->write(1);
		xSemaphoreGive(_mutex);
	}
	return 1;
}

unsigned char Si4463::ReadFRR_ISR(unsigned char* DATA)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if(xSemaphoreTakeFromISR(_mutex, &xHigherPriorityTaskWoken) == pdTRUE)
	{
		_nsel->write(0); 							// NSEL is LOW
		_spi->write(0x50);
		for(unsigned char i = 0; i < 4; i++)
		{
			DATA[i] = _spi->write(0xFF);
		}
		_nsel->write(1);
		xSemaphoreGiveFromISR(_mutex, &xHigherPriorityTaskWoken);
	}
	return 1;
}

unsigned char Si4463::WriteTXFIFO(unsigned char* DATA, uint16_t LENGTH)
{
	if(xSemaphoreTake(_mutex, portMAX_DELAY) == pdTRUE)
	{
		_nsel->write(0); 							// NSEL is LOW
		_spi->write(0x66);
		for(unsigned char i = 0; i < LENGTH; i++)
		{
			_spi->write(DATA[i]);
		}
		_nsel->write(1);
		xSemaphoreGive(_mutex);
	}
	return 1;
}

unsigned char Si4463::ReadRXFIFO(unsigned char* DATA, uint16_t LENGTH)
{
	if(xSemaphoreTake(_mutex, portMAX_DELAY) == pdTRUE)
	{
		_nsel->write(0); 							// NSEL is LOW
		_spi->write(0x77);
		for(unsigned char i = 0; i < LENGTH; i++)
		{
			DATA[i] = _spi->write(0xFF);
		}
		_nsel->write(1);
		xSemaphoreGive(_mutex);
	}
	return 1;
}

unsigned char Si4463::ReadRXFIFO_ISR(unsigned char* DATA, uint16_t LENGTH)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if(xSemaphoreTakeFromISR(_mutex, &xHigherPriorityTaskWoken) == pdTRUE)
	{
		_nsel->write(0); 							// NSEL is LOW
		_spi->write(0x77);
		for(unsigned char i = 0; i < LENGTH; i++)
		{
			DATA[i] = _spi->write(0xFF);
		}
		_nsel->write(1);
		xSemaphoreGiveFromISR(_mutex, &xHigherPriorityTaskWoken);
	}
	return 1;
}

unsigned char Si4463::SetFrequency(unsigned int FREQ)
{
    // Set the output divider according to recommended ranges given in Si446x datasheet

    int outdiv = 4;
    int band = 0;
    if (FREQ < 705000000UL) { outdiv = 6;  band = 1;};
    if (FREQ < 525000000UL) { outdiv = 8;  band = 2;};
    if (FREQ < 353000000UL) { outdiv = 12; band = 3;};
    if (FREQ < 239000000UL) { outdiv = 16; band = 4;};
    if (FREQ < 177000000UL) { outdiv = 24; band = 5;};

    unsigned long f_pfd = 2 * 30000000 / outdiv; // CRYSTAL FREQUENCY = 30000000
    unsigned int n = ((unsigned int)(FREQ / f_pfd)) - 1;
    float ratio = (float)FREQ / (float)f_pfd;
    float rest  = ratio - (float)n;
    unsigned long m = (unsigned long)(rest * 524288UL);
    // set the band parameter
    unsigned int sy_sel = 8; //
    unsigned char set_band_property_command[] = {0x11, 0x20, 0x01, 0x51, (band + sy_sel)}; // CLKGEN_BAND
    // send parameters
    if(!_cmd(set_band_property_command, 5, 0)) return 0;
    // Set the pll parameters
    unsigned int m2 = m / 0x10000;
    unsigned int m1 = (m - m2 * 0x10000) / 0x100;
    unsigned int m0 = (m - m2 * 0x10000 - m1 * 0x100);
    // assemble parameter string
    unsigned char set_frequency_property_command[] = {0x11, 0x40, 0x04, 0x00, n, m2, m1, m0};
    // send parameters
    if(!_cmd(set_frequency_property_command, 8, 0)) return 0;
	return 1;
}

unsigned char Si4463::SetTXModem()
{
	/*GLOBAL_XO_TUNE*/
	unsigned char Buffer[5] = {RF_GLOBAL_XO_TUNE_1};
	if(!_cmd(Buffer, 5, 0)) return 0;

	/*GLOBAL_CONFIG*/
	unsigned char Buffer2[5] = {RF_GLOBAL_CONFIG_1};
	if(!_cmd(Buffer2, 5, 0)) return 0;

	/*INTERRUPT_CONTROL*/
	unsigned char Buffer3[6] = {RF_INT_CTL_ENABLE_2};
	if(!_cmd(Buffer3, 6, 0)) return 0;

	/*PREAMBLE_TX_LENGTH*/
	unsigned char Buffer4[5] = {RF_PREAMBLE_TX_LENGTH_1};
	if(!_cmd(Buffer4, 5, 0)) return 0;

	/*PREAMBLE_CONFIG*/
	unsigned char Buffer5[5] = {RF_PREAMBLE_CONFIG_1};
	if(!_cmd(Buffer5, 5, 0)) return 0;

	/*SYNC_CONFIG*/
	unsigned char Buffer6[7] = {RF_SYNC_CONFIG_3};
	if(!_cmd(Buffer6, 7, 0)) return 0;

	/*PACKET_CONFIG1*/
	unsigned char Buffer7[5] = {RF_PKT_CONFIG1_1};
	if(!_cmd(Buffer7, 5, 0)) return 0;

    /*RF_PKT_FIELD_1_CONFIG*/
    //unsigned char Buffer8[5] = {RF_PKT_FIELD_1_CONFIG_1};
    //if(!SendCMDReceiveAnswer(Buffer8, 5, 0)) return 0;
    /*RF_PKT_FIELD_1_CONFIG*/
    unsigned char Buffer8[16] = {RF_PKT_LEN_12};
    if(!_cmd(Buffer8, 16, 0)) return 0;


	/*RF_MODEM_MOD_TYPE_12*/
	unsigned char Buffer9[16] = {RF_MODEM_MOD_TYPE_12};
	if(!_cmd(Buffer9, 16, 0)) return 0;

	/*RF_MODEM_FREQ_DEV_0*/
	unsigned char Buffer10[5] = {RF_MODEM_FREQ_DEV_0_1};
	if(!_cmd(Buffer10, 5, 0)) return 0;

	/*RF_MODEM_TX_RAMP_DELAY_8*/
	unsigned char Buffer11[12] = {RF_MODEM_TX_RAMP_DELAY_8};
	if(!_cmd(Buffer11, 12, 0)) return 0;

	/*RF_MODEM_BCR_OSR_1*/
	unsigned char Buffer12[13] = {RF_MODEM_BCR_OSR_1_9};
	if(!_cmd(Buffer12, 13, 0)) return 0;

	/*RF_MODEM_AFC_GEAR*/
	unsigned char Buffer13[11] = {RF_MODEM_AFC_GEAR_7};
	if(!_cmd(Buffer13, 11, 0)) return 0;

	/*RF_MODEM_AGC_CONTROL_1*/
	unsigned char Buffer14[5] = {RF_MODEM_AGC_CONTROL_1};
	if(!_cmd(Buffer14, 5, 0)) return 0;

	/*RF_MODEM_AGC_WINDOW_SIZE_9*/
	unsigned char Buffer15[13] = {RF_MODEM_AGC_WINDOW_SIZE_9};
	if(!_cmd(Buffer15, 13, 0)) return 0;

	/*RF_MODEM_OOK_CNT1_8*/
	unsigned char Buffer16[12] = {RF_MODEM_OOK_CNT1_8};
	if(!_cmd(Buffer16, 12, 0)) return 0;

	/*RF_MODEM_RSSI_COMP_1*/
	unsigned char Buffer17[5] = {RF_MODEM_RSSI_COMP_1};
	if(!_cmd(Buffer17, 5, 0)) return 0;

	/*RF_MODEM_CLKGEN_BAND_1*/
	unsigned char Buffer18[5] = {RF_MODEM_CLKGEN_BAND_1};
	if(!_cmd(Buffer18, 5, 0)) return 0;

	/*RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12*/
	unsigned char Buffer19[16] = {RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12};
	if(!_cmd(Buffer19, 16, 0)) return 0;

	/*RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12*/
	unsigned char Buffer20[16] = {RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12};
	if(!_cmd(Buffer20, 16, 0)) return 0;

	/*RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12*/
	unsigned char Buffer21[16] = {RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12};
	if(!_cmd(Buffer21, 16, 0)) return 0;

	/*RF_SYNTH_PFDCP_CPFF_7*/
	unsigned char Buffer22[11] = {RF_SYNTH_PFDCP_CPFF_7};
	if(!_cmd(Buffer22, 11, 0)) return 0;

	/*RF_FREQ_CONTROL_INTE_8*/
	unsigned char Buffer23[12] = {RF_FREQ_CONTROL_INTE_8};
	if(!_cmd(Buffer23, 12, 0)) return 0;
	return 1;
}

unsigned char Si4463::SetRXModem()
{
	/*GLOBAL_XO_TUNE*/
	unsigned char Buffer[5] = {RF_GLOBAL_XO_TUNE_1};
	if(!_cmd(Buffer, 5, 0)) return 0;

	/*GLOBAL_CONFIG*/
	unsigned char Buffer2[5] = {RF_GLOBAL_CONFIG_1};
	if(!_cmd(Buffer2, 5, 0)) return 0;

	/*INTERRUPT_CONTROL*/
	unsigned char Buffer3[7] = {RF_INT_CTL_ENABLE_3};
	if(!_cmd(Buffer3, 7, 0)) return 0;

	/*PREAMBLE_TX_LENGTH*/
	/*unsigned char Buffer4[5] = {RF_PREAMBLE_TX_LENGTH_1};
	if(!SendCMDReceiveAnswer(Buffer4, 5, 0)) return 0;*/

	/*PREAMBLE_CONFIG*/
	unsigned char Buffer5[5] = {RF_PREAMBLE_CONFIG_1};
	if(!_cmd(Buffer5, 5, 0)) return 0;

	/*SYNC_CONFIG*/
	unsigned char Buffer6[7] = {RF_SYNC_CONFIG_3};
	if(!_cmd(Buffer6, 7, 0)) return 0;

	/*PACKET_CONFIG1*/
	unsigned char Buffer7[5] = {RF_PKT_CONFIG1_1};
	if(!_cmd(Buffer7, 5, 0)) return 0;

	/*RF_PKT_FIELD_1_CONFIG*/
	//unsigned char Buffer8[5] = {RF_PKT_FIELD_1_CONFIG_1};
	//if(!SendCMDReceiveAnswer(Buffer8, 5, 0)) return 0;
	/*RF_PKT_FIELD_1_CONFIG*/
	unsigned char Buffer8[16] = {RF_PKT_LEN_12};
	if(!_cmd(Buffer8, 16, 0)) return 0;

	/*RF_MODEM_MOD_TYPE_12*/
	unsigned char Buffer9[16] = {RF_MODEM_MOD_TYPE_12};
	if(!_cmd(Buffer9, 16, 0)) return 0;

	/*RF_MODEM_FREQ_DEV_0*/
	unsigned char Buffer10[5] = {RF_MODEM_FREQ_DEV_0_1};
	if(!_cmd(Buffer10, 5, 0)) return 0;

	/*RF_MODEM_TX_RAMP_DELAY_8*/
	unsigned char Buffer11[12] = {RF_MODEM_TX_RAMP_DELAY_8};
	if(!_cmd(Buffer11, 12, 0)) return 0;

	/*RF_MODEM_BCR_OSR_1*/
	unsigned char Buffer12[13] = {RF_MODEM_BCR_OSR_1_9};
	if(!_cmd(Buffer12, 13, 0)) return 0;

	/*RF_MODEM_AFC_GEAR*/
	unsigned char Buffer13[11] = {RF_MODEM_AFC_GEAR_7};
	if(!_cmd(Buffer13, 11, 0)) return 0;

	/*RF_MODEM_AGC_CONTROL_1*/
	unsigned char Buffer14[5] = {RF_MODEM_AGC_CONTROL_1};
	if(!_cmd(Buffer14, 5, 0)) return 0;

	/*RF_MODEM_AGC_WINDOW_SIZE_9*/
	unsigned char Buffer15[13] = {RF_MODEM_AGC_WINDOW_SIZE_9};
	if(!_cmd(Buffer15, 13, 0)) return 0;

	/*RF_MODEM_OOK_CNT1_11*/
	unsigned char Buffer16[15] = {RF_MODEM_OOK_CNT1_11};
	if(!_cmd(Buffer16, 15, 0)) return 0;

	/*RF_MODEM_RSSI_COMP_1*/
	unsigned char Buffer17[5] = {RF_MODEM_RSSI_COMP_1};
	if(!_cmd(Buffer17, 5, 0)) return 0;

	/*RF_MODEM_CLKGEN_BAND_1*/
	unsigned char Buffer18[5] = {RF_MODEM_CLKGEN_BAND_1};
	if(!_cmd(Buffer18, 5, 0)) return 0;

	/*RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12*/
	unsigned char Buffer19[16] = {RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12};
	if(!_cmd(Buffer19, 16, 0)) return 0;

	/*RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12*/
	unsigned char Buffer20[16] = {RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12};
	if(!_cmd(Buffer20, 16, 0)) return 0;

	/*RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12*/
	unsigned char Buffer21[16] = {RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12};
	if(!_cmd(Buffer21, 16, 0)) return 0;

	/*RF_SYNTH_PFDCP_CPFF_7*/
	unsigned char Buffer22[11] = {RF_SYNTH_PFDCP_CPFF_7};
	if(!_cmd(Buffer22, 11, 0)) return 0;

	/*RF_FREQ_CONTROL_INTE_8*/
	unsigned char Buffer23[12] = {RF_FREQ_CONTROL_INTE_8};
	if(!_cmd(Buffer23, 12, 0)) return 0;
	return 1;
}

unsigned char Si4463::SetFRR(FRR_CTL_MODE FRR_A, FRR_CTL_MODE FRR_B, FRR_CTL_MODE FRR_C, FRR_CTL_MODE FRR_D)
{
	unsigned char Buffer[8] = {0x11, 0x02, 0x04, 0x00, FRR_A, FRR_B, FRR_C, FRR_D};
	if(!_cmd(Buffer, 8, 0)) return 0;
	return 1;
}

unsigned char Si4463::SetPA(unsigned char PA)
{
	unsigned char BUFFER1[8] = {0x11, 0x22, 0x04, 0x00, 0x08, PA, 0x00, 0x1D};
	if(!_cmd(BUFFER1, 8, 0)) return 0;
	return 1;
}

unsigned char Si4463::Broadcast(unsigned char* DATA, uint16_t LENGTH)
{
	if(!WriteTXFIFO(DATA, LENGTH))
		return 0;
	if(!StartTX(LENGTH))
		return 0;
	uint16_t timeout = 500;
	uint16_t counter = 0;
	unsigned char success = 0;
	while(!success && counter < timeout)
	{
		unsigned char PCK_SENT[4];
		ReadFRR(PCK_SENT);
		if(PCK_SENT[0] & (0x01 << 5))
		{
			success = 1;
		}
		counter++;
		vTaskDelay(1);
	}
	ClearInterrupts();
	return success;
}

unsigned char Si4463::ClearInterrupts()
{
	unsigned char Buffer[8] = {0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	if(!_cmd(Buffer, 4, 8)) return 0;
	return 1;
}

unsigned char Si4463::ClearInterrupts_ISR(portBASE_TYPE* xHigherPriorityTaskWoken)
{
	unsigned char Buffer[8] = {0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	if(!_cmd_ISR(Buffer, 4, 8, xHigherPriorityTaskWoken)) return 0;
	return 1;
}

unsigned char Si4463::SendData(unsigned char* DATA, unsigned int LENGTH)
{
	unsigned char Buffer[64] = {0};

	unsigned int TotalPacketNumber = 0;
	unsigned int CurrentPacketNumber = 0;
	unsigned int TotalByteNumber = 48 + 3;
	unsigned int Residue = 0;
	unsigned char CharTotalPacketNumber[2];
	unsigned char CharCurrentPacketNumber[2];
	unsigned char CharTotalByteNumber[2];
	unsigned char CharResidue[2];

	unsigned char index = 0;

	Residue = LENGTH % 48;
	Residue += 3; // END

	if(Residue == 0) TotalPacketNumber = LENGTH / 48;
	else TotalPacketNumber = LENGTH / 48 + 1;

	*(unsigned int*) CharTotalPacketNumber = TotalPacketNumber;
	*(unsigned int*) CharCurrentPacketNumber = CurrentPacketNumber;
	*(unsigned int*) CharTotalByteNumber = TotalByteNumber;
	*(unsigned int*) CharResidue = Residue;

	for(CurrentPacketNumber = 1; CurrentPacketNumber <= TotalPacketNumber; CurrentPacketNumber++)
	{
		*(unsigned int*) CharCurrentPacketNumber = CurrentPacketNumber;
		index = 0;
		Buffer[index++] = 'S';
		Buffer[index++] = 'T';
		Buffer[index++] = 'A';
		Buffer[index++] = 'R';
		Buffer[index++] = 'T';

		Buffer[index++] = CharTotalPacketNumber[0];
		Buffer[index++] = CharTotalPacketNumber[1];

		Buffer[index++] = CharCurrentPacketNumber[0];
		Buffer[index++] = CharCurrentPacketNumber[1];

		if(CurrentPacketNumber == TotalPacketNumber && Residue != 0)
		{
			Buffer[index++] = CharResidue[0];
			Buffer[index++] = CharResidue[1];
			Buffer[index++] = 'O';
			Buffer[index++] = 'K';

			for(int i = 0; i < (Residue - 3); i++)
			{
				Buffer[index++] = DATA[(CurrentPacketNumber - 1)*48 + i];
			}

			Buffer[index++] = 'E';
			Buffer[index++] = 'N';
			Buffer[index++] = 'D';
		}
		else
		{
			Buffer[index++] = CharTotalByteNumber[0];
			Buffer[index++] = CharTotalByteNumber[1];
			Buffer[index++] = 'O';
			Buffer[index++] = 'K';

			for(int i = 0; i < (TotalByteNumber - 3); i++)
			{
				Buffer[index++] = DATA[(CurrentPacketNumber - 1)*48 + i];
			}

			Buffer[index++] = 'E';
			Buffer[index++] = 'N';
			Buffer[index++] = 'D';
		}

		Broadcast(Buffer, 64);
		//delay(500);
	}

	return 1;
}

unsigned char Si4463::_cmd(unsigned char* BUFFER, unsigned char CMD_LENGTH, unsigned char RES_LENGTH)
{
	if(xSemaphoreTake(_mutex, portMAX_DELAY) == pdTRUE)
	{
		_nsel->write(0); 							// NSEL is LOW
		//********************************************************************
		// Send command ******************************************************
		for(unsigned char i = 0; i < CMD_LENGTH; i++)
		{
			_spi->write(BUFFER[i]);
		}
		//********************************************************************
		//********************************************************************
		_nsel->write(1); 							// NSEL is HIGH
		vTaskDelay(1);
		_nsel->write(0);

		//********************************************************************
		// Receive CTS *******************************************************
		unsigned char CTS_CMD = 0x44;		// READ_CMD_BUFF command (AN625)
		unsigned char CTS_RES = 0;
		unsigned int timeout_counter = 0;
		while(CTS_RES != 0xFF && timeout_counter < _timeout)
		{
			_spi->write(CTS_CMD);
			CTS_RES = _spi->write(0xFF);
			timeout_counter++;
			if(CTS_RES != 0xFF)
			{
				_nsel->write(1);
				vTaskDelay(1);
				_nsel->write(0);
			}
		}
		if(timeout_counter >= _timeout)
		{
			xSemaphoreGive(_mutex);
			return 0;
		}
		//********************************************************************
		//********************************************************************

		//********************************************************************
		// Receive response **************************************************
		for(unsigned char i = 0; i < RES_LENGTH; i++)
		{
			BUFFER[i] = _spi->write(0xFF);
		}
		//********************************************************************
		//********************************************************************

		_nsel->write(1);
		xSemaphoreGive(_mutex);
	}
	return 1;
}

unsigned char Si4463::_cmd_ISR(unsigned char* BUFFER, unsigned char CMD_LENGTH, unsigned char RES_LENGTH, portBASE_TYPE* xHigherPriorityTaskWoken)
{
	if(xSemaphoreTakeFromISR(_mutex, xHigherPriorityTaskWoken) == pdTRUE)
	{
		_nsel->write(0); 							// NSEL is LOW
		//********************************************************************
		// Send command ******************************************************
		for(unsigned char i = 0; i < CMD_LENGTH; i++)
		{
			_spi->write(BUFFER[i]);
		}
		//********************************************************************
		//********************************************************************
		_nsel->write(1); 							// NSEL is HIGH
		vTaskDelay(1);
		_nsel->write(0);

		//********************************************************************
		// Receive CTS *******************************************************
		unsigned char CTS_CMD = 0x44;		// READ_CMD_BUFF command (AN625)
		unsigned char CTS_RES = 0;
		unsigned int timeout_counter = 0;
		while(CTS_RES != 0xFF && timeout_counter < _timeout)
		{
			_spi->write(CTS_CMD);
			CTS_RES = _spi->write(0xFF);
			timeout_counter++;
			if(CTS_RES != 0xFF)
			{
				_nsel->write(1);
				vTaskDelay(1);
				_nsel->write(0);
			}
		}
		if(timeout_counter >= _timeout)
		{
			xSemaphoreGiveFromISR(_mutex, xHigherPriorityTaskWoken);
			return 0;
		}
		//********************************************************************
		//********************************************************************

		//********************************************************************
		// Receive response **************************************************
		for(unsigned char i = 0; i < RES_LENGTH; i++)
		{
			BUFFER[i] = _spi->write(0xFF);
		}
		//********************************************************************
		//********************************************************************

		_nsel->write(1);
		xSemaphoreGiveFromISR(_mutex, xHigherPriorityTaskWoken);
	}
	return 1;
}

