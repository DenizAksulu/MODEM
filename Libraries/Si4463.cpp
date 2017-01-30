/*
 * Si4463.cpp
 *
 *  Created on: 30 Oca 2017
 *      Author: Mehmet Deniz Aksulu
 */

#include "Si4463.h"

using namespace MODEM;

Si4463::Si4463(SPI* spi, DigitalOut* nsel, DigitalOut* sdn)
{
	_spi = spi;							// SPI
	_nsel = nsel;						// NSEL pin
	_sdn = sdn;							// SDN pin
	_sdn->write(1);						// SDN is LOW
	_nsel->write(1);					// NSEL is HIGH
	_mutex = xSemaphoreCreateMutex();	// Create MUTEX
	_timeout = 10000;					// Command timeout in us
	_spi->frequency(500000);			// 1 MHz
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

