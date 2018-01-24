/*
 * PC.cpp
 *
 *  Created on: 6 Mar 2017
 *      Author: Mehmet Deniz Aksulu
 */

#include "PC.h"

using namespace MODEM;

Serial* _uart;

const unsigned char HEADER_LENGTH = 7;
const unsigned char FOOTER_LENGTH = 3;

unsigned char RX_BUFFER[512] = {0};
unsigned char* _RX_DATA;

void UARTHandler();

PC::PC(PinName TX, PinName RX)
{
	_uart = new Serial(TX, RX);
	_uart->baud(115200);
	_uart->attach(&UARTHandler, Serial::RxIrq);
}

unsigned char PC::Send(unsigned char* DATA, uint16_t LENGTH)
{
	return write_multiple(AssemblePacket(DATA, LENGTH), LENGTH + 10);
}

unsigned char PC::write_multiple(unsigned char* DATA, uint16_t LENGTH)
{
	for(int i = 0; i < LENGTH; i++)
	{
		_uart->putc(DATA[i]);
	}
	return 1;
}

unsigned char* BUFFER;
unsigned char* PC::AssemblePacket(unsigned char* DATA, uint16_t LENGTH)
{
	BUFFER = new unsigned char[HEADER_LENGTH + FOOTER_LENGTH + LENGTH];
	int i = 0;
	BUFFER[i++] = 'A';
	BUFFER[i++] = 'C';
	BUFFER[i++] = 'Q';
	unsigned char LENGTH_BUFFER[2] = {0};
	*(uint16_t*)LENGTH_BUFFER = LENGTH + 3;
	BUFFER[i++] = LENGTH_BUFFER[0];
	BUFFER[i++] = LENGTH_BUFFER[1];
	BUFFER[i++] = 'O';
	BUFFER[i++] = 'K';
	for(int j = 0; j < LENGTH; j++)
	{
		BUFFER[i++] = DATA[j];
	}
	BUFFER[i++] = 'E';
	BUFFER[i++] = 'N';
	BUFFER[i++] = 'D';

	return BUFFER;
}

unsigned char UART_MODE = 0;
int data_counter = 0;
uint16_t data_length = 0;
void UARTHandler()
{
	RX_BUFFER[data_counter++] = _uart->getc();
	switch(UART_MODE)
	{
	case 0:
		if(data_counter == 1 && RX_BUFFER[data_counter - 1] != 'A')
		{
			data_counter = 0;
			return;
		}
		else if(data_counter == 2 && RX_BUFFER[data_counter - 1] != 'C')
		{
			data_counter = 0;
			return;
		}
		else if(data_counter == 3 && RX_BUFFER[data_counter - 1] != 'Q')
		{
			data_counter = 0;
			return;
		}
		else if(data_counter == 6 && RX_BUFFER[data_counter - 1] != 'O')
		{
			data_counter = 0;
			return;
		}
		else if(data_counter == 7 && RX_BUFFER[data_counter - 1] != 'K')
		{
			data_counter = 0;
			return;
		}
		if(data_counter == 7)
		{
			data_length = RX_BUFFER[3] + RX_BUFFER[4]*256;
			data_counter = 0;
			UART_MODE = 1;
		}
		break;
	case 1:
		if(data_counter == data_length)
		{
			if(RX_BUFFER[data_length - 3] == 'E' && RX_BUFFER[data_length - 2] == 'N' && RX_BUFFER[data_length - 1] == 'D')
			{

				_RX_DATA = new unsigned char[data_length - 3];
				for(int i = 0; i < data_length - 3; i++)
				{
					_RX_DATA[i] = RX_BUFFER[i];
				}
				data_counter = 0;
				UART_MODE = 0;
				portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
				xQueueSendFromISR(QUEUE_PC_DATA, &_RX_DATA, &xHigherPriorityTaskWoken);
				xQueueSendFromISR(QUEUE_PC_DATA_LENGTH, &data_length, &xHigherPriorityTaskWoken);
				xSemaphoreGiveFromISR(BINARY_PC, &xHigherPriorityTaskWoken);
				
				if (xHigherPriorityTaskWoken != pdFALSE)
				{
					portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
				}
			}
			else
			{
				data_counter = 0;
				UART_MODE = 0;
				return;
			}
		}
		break;
	}
}

