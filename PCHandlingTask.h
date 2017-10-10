/*
 * PCHandlingTask.h
 *
 *  Created on: 6 Mar 2017
 *      Author: Mehmet Deniz Aksulu
 */

#ifndef PCHANDLINGTASK_H_
#define PCHANDLINGTASK_H_

#include "MODEM.h"

#define PC_COMMAND_CONNECT 		0x01
#define PC_COMMAND_MSP_STATUS  	0x02
#define PC_COMMAND_RX_STATUS  	0x03
#define PC_COMMAND_TX_STATUS  	0x04
#define PC_COMMAND_RX_TEMP		0x05
#define PC_COMMAND_TX_TEMP		0x06
#define PC_COMMAND_GET_RX_FREQ	0x07
#define PC_COMMAND_GET_TX_FREQ	0x08
#define PC_COMMAND_GET_PA		0x09
#define PC_COMMAND_SET_PA		0x10
#define PC_COMMAND_TRANSMIT		0x11

#define PC_ACK 0xAA
#define PC_NACK 0xFF

void PCHandlingTaskCode(void* Parameters);


#endif /* PCHANDLINGTASK_H_ */
