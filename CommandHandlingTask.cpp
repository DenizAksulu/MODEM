/*
 * CommandHandlingTask.cpp
 *
 *  Created on: 4 Mar 2017
 *      Author: Mehmet Deniz Aksulu
 */

#include "CommandHandlingTask.h"
#include "Si4463.h"

using namespace MODEM;

void CommandHandlingTaskCode(void* Parameters)
{
	unsigned int totalLength = 0;
	unsigned int TotalReceivedDataLengthModem = 0;
	unsigned char ReceivedData[512] = {0};
	unsigned char RX_FIFO[256] = {0};
	DBG("Command handling start.")
	Si4463* rx_modem;
	xQueuePeek(QUEUE_RX_OBJECT, &rx_modem, portMAX_DELAY);
	while(1)
	{
		xSemaphoreTake(BINARY_MODEM, portMAX_DELAY);
		DBG("Command received.")
		unsigned char DataBuffer[64] = {};
		unsigned int PacketLength = 0;
		unsigned int TotalPacketNumber = 0;
		unsigned int CurrentPacketNumber = 0;
		bool success = 0;
		unsigned long timeOutcounter = 0;
		while(!success)
		{
			unsigned char PCK_SENT[4];
			timeOutcounter++;
			rx_modem->ReadFRR(PCK_SENT);
			if(PCK_SENT[1] & 0x10)
			{
				success = 1;
			}
			if(timeOutcounter >= 250000)
			{
				timeOutcounter = 0;
				return;
			}
			vTaskDelay(10);
		}
		rx_modem->ReadRXFIFO(DataBuffer, 64);
		rx_modem->ChangeState(SPI_ACTIVE);
		rx_modem->ClearInterrupts();

		if(DataBuffer[0] == 'S' && DataBuffer[1] == 'T' && DataBuffer[2] == 'A' && DataBuffer[3] == 'R' && DataBuffer[4] == 'T'
				&& DataBuffer[11] == 'O' && DataBuffer[12] == 'K')
		{
			TotalPacketNumber = DataBuffer[5] + DataBuffer[6]*256;
			CurrentPacketNumber = DataBuffer[7] + DataBuffer[8]*256;
			PacketLength = DataBuffer[9] + DataBuffer[10]*256;
			if(DataBuffer[PacketLength + 13 - 1] == 'D' && DataBuffer[PacketLength + 13 - 2] == 'N' && DataBuffer[PacketLength + 13 - 3] == 'E')
			{
				for(int i = 0; i < PacketLength - 3; i++)
				{
					ReceivedData[48*(CurrentPacketNumber - 1) + i] = DataBuffer[13 + i];
				}
				TotalReceivedDataLengthModem += PacketLength - 3;

				if(CurrentPacketNumber == TotalPacketNumber)
				{
					for(int m = 0; m < 256; m++)
					{
						RX_FIFO[m] = 0;
					}
					for(int j = 0; j < TotalReceivedDataLengthModem; j++)
					{
						RX_FIFO[j] = ReceivedData[j];
					}
					DBG("%s", RX_FIFO);
					TotalReceivedDataLengthModem = 0;
				}
			}
		}
		rx_modem->StartRX(64);
	}
}
