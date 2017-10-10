/*
 * PC.h
 *
 *  Created on: 6 Mar 2017
 *      Author: Mehmet Deniz Aksulu
 */

#ifndef LIBRARIES_PC_H_
#define LIBRARIES_PC_H_

#include "../MODEM.h"

class PC
{
public:
	PC(PinName TX, PinName RX);
	unsigned char Send(unsigned char* DATA, uint16_t LENGTH);
private:
	unsigned char* AssemblePacket(unsigned char* DATA, uint16_t LENGTH);
	unsigned char write_multiple(unsigned char* DATA, uint16_t LENGTH);
};

#endif /* LIBRARIES_PC_H_ */
