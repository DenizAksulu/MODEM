/*
 * PCHandlingTask.cpp
 *
 *  Created on: 6 Mar 2017
 *      Author: Mehmet Deniz Aksulu
 */

#include "PCHandlingTask.h"
#include "PC.h"
#include "Si4463.h"

using namespace MODEM;

void PCHandlingTaskCode(void* Parameters)
{
	PC pc(PA_9, PA_10);
	unsigned char* _RX_DATA;
	unsigned char* _TX_DATA;
	Si4463* tx_modem;
	STRUCT_MODEM_STATUS* rx_modem_status;
	STRUCT_MODEM_STATUS* tx_modem_status;
	uint16_t _RX_DATA_LENGTH;
	xQueuePeek(QUEUE_TX_OBJECT, &tx_modem, portMAX_DELAY);
	xQueuePeek(QUEUE_RX_STATUS, &rx_modem_status, portMAX_DELAY);
	xQueuePeek(QUEUE_TX_STATUS, &tx_modem_status, portMAX_DELAY);
	while(1)
	{
		xSemaphoreTake(BINARY_PC, portMAX_DELAY);
		xQueueReceive(QUEUE_PC_DATA, &_RX_DATA, portMAX_DELAY);
		xQueueReceive(QUEUE_PC_DATA_LENGTH, &_RX_DATA_LENGTH, portMAX_DELAY);
		LED_RED = !LED_RED;
		switch(_RX_DATA[0])
		{
		case PC_COMMAND_CONNECT:
			_TX_DATA = new unsigned char[2];
			_TX_DATA[0] = _RX_DATA[0];
			_TX_DATA[1] = PC_ACK;
			pc.Send(_TX_DATA, 2);
			delete[] _TX_DATA;
			break;
		case PC_COMMAND_MSP_STATUS:
			_TX_DATA = new unsigned char[2];
			_TX_DATA[0] = _RX_DATA[0];
			_TX_DATA[1] = PC_ACK;
			pc.Send(_TX_DATA, 2);
			delete[] _TX_DATA;
			break;
		case PC_COMMAND_RX_STATUS:
			_TX_DATA = new unsigned char[2];
			_TX_DATA[0] = _RX_DATA[0];
			if(rx_modem_status->status)
				_TX_DATA[1] = PC_ACK;
			else
				_TX_DATA[1] = PC_NACK;
			pc.Send(_TX_DATA, 2);
			delete[] _TX_DATA;
			break;
		case PC_COMMAND_TX_STATUS:
			_TX_DATA = new unsigned char[2];
			_TX_DATA[0] = _RX_DATA[0];
			if(tx_modem_status->status)
				_TX_DATA[1] = PC_ACK;
			else
				_TX_DATA[1] = PC_NACK;
			pc.Send(_TX_DATA, 2);
			delete[] _TX_DATA;
			break;
		case PC_COMMAND_RX_TEMP:
			_TX_DATA = new unsigned char[5];
			_TX_DATA[0] = _RX_DATA[0];
			for(int i = 0; i < 4; i++)
			{
				_TX_DATA[i + 1] = rx_modem_status->temperature[i];
			}
			pc.Send(_TX_DATA, 5);
			delete[] _TX_DATA;
			break;
		case PC_COMMAND_TX_TEMP:
			_TX_DATA = new unsigned char[5];
			_TX_DATA[0] = _RX_DATA[0];
			for(int i = 0; i < 4; i++)
			{
				_TX_DATA[i + 1] = tx_modem_status->temperature[i];
			}
			pc.Send(_TX_DATA, 5);
			delete[] _TX_DATA;
			break;
		case PC_COMMAND_TRANSMIT:
			unsigned char* TRANSMIT_BUFFER = new unsigned char[_RX_DATA_LENGTH - 1];
			for(int i = 0; i < _RX_DATA_LENGTH - 1; i++)
			{
				TRANSMIT_BUFFER[i] = _RX_DATA[i + 1];
			}
			tx_modem->SendData(TRANSMIT_BUFFER, _RX_DATA_LENGTH - 1);
			_TX_DATA = new unsigned char[2];
			_TX_DATA[0] = _RX_DATA[0];
			_TX_DATA[1] = PC_ACK;
			pc.Send(_TX_DATA, 2);
			delete[] _TX_DATA;
			delete[] TRANSMIT_BUFFER;
			break;
		case PC_COMMAND_TRANSMIT_BEACON:
			unsigned char* TRANSMIT_BUFFER = new unsigned char[_RX_DATA_LENGTH - 1];
			for(int i = 0; i < _RX_DATA_LENGTH - 1; i++)
			{
				TRANSMIT_BUFFER[i] = _RX_DATA[i + 1];
			}
			tx_modem->BroadcastBeacon(TRANSMIT_BUFFER, _RX_DATA_LENGTH - 1);
			_TX_DATA = new unsigned char[2];
			_TX_DATA[0] = _RX_DATA[0];
			_TX_DATA[1] = PC_ACK;
			pc.Send(_TX_DATA, 2);
			delete[] _TX_DATA;
			delete[] TRANSMIT_BUFFER;
			break;
		case PC_COMMAND_SET_DASH_BEACON:
			unsigned int dash = _RX_DATA[1] + _RX_DATA[2]*256 + _RX_DATA[3]*256*256+ _RX_DATA[4]*256*256*256;
			tx_modem->dash = dash;
			_TX_DATA = new unsigned char[2];
			_TX_DATA[0] = _RX_DATA[0];
			_TX_DATA[1] = PC_ACK;
			pc.Send(_TX_DATA, 2);
			delete[] _TX_DATA;
			delete[] TRANSMIT_BUFFER;
			break;
		}
	}
}



