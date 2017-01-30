/*
 * Si4463.h
 *
 *  Created on: 30 Oca 2017
 *      Author: Mehmet Deniz Aksulu
 */

#ifndef LIBRARIES_SI4463_H_
#define LIBRARIES_SI4463_H_

#include "../MODEM.h"

class Si4463
{
public:
	Si4463(SPI* spi, DigitalOut* nsel, DigitalOut* sdn);
	unsigned char PowerUp();
	unsigned char NOP();
	unsigned char PartInfo(unsigned char* INFO);
	unsigned char Temperature(unsigned char* TEMPERATURE);
	unsigned char Temperature(double* TEMPERATURE);

private:
	unsigned char _cmd(unsigned char* BUFFER, unsigned char CMD_LENGTH, unsigned char RES_LENGTH);
	xSemaphoreHandle _mutex;
	SPI* _spi;
	DigitalOut* _nsel;
	DigitalOut* _sdn;
	unsigned int _timeout;
};

#endif /* LIBRARIES_SI4463_H_ */
