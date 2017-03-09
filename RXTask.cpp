/*
 * RXTask.cpp
 *
 *  Created on: 30 Oca 2017
 *      Author: Mehmet Deniz Aksulu
 */

#include "RXTask.h"
#include "Si4463.h"

using namespace MODEM;

void RXTaskCode(void* Parameters)
{
	DBG("RX task start.");

	unsigned char TEMP[4];
	unsigned char FREQ[4];
	STRUCT_MODEM_STATUS* modem_status = new STRUCT_MODEM_STATUS;
	xQueueSend(QUEUE_RX_STATUS, &modem_status, portMAX_DELAY);

	SPI* rx_modem_spi = new SPI(PC_12, PC_11, PC_10);						// SPI for RX modem
	DigitalOut* rx_modem_nsel = new DigitalOut(PC_9, 1);					// NSEL for RX modem
	DigitalOut* rx_modem_sdn =  new DigitalOut(PD_13, 1);					// SDN for RX modem
	Si4463* rx_modem = new Si4463(rx_modem_spi, rx_modem_nsel, rx_modem_sdn);
	xQueueSend(QUEUE_RX_OBJECT, &rx_modem, portMAX_DELAY);					// Send rx modem object

	DBG("Power up RX modem...");
	if(!rx_modem->PowerUp())
	{
		DBG("RX modem power up failed!");
	}
	DBG("Power up successful!")

	rx_modem->SetGPIO(SYNC, VALID_PREAMBLE, RX); //OK!
	/*FRR Settings*/
	rx_modem->SetFRR(PACKET_HANDLER_PENDING, PACKET_HANDLER_STATUS, MODEM_PENDING, CHIP_PENDING); // OK!

	if(!rx_modem->SetRXModem())
	{
		DBG("Set RX modem failed!");
	}

	rx_modem->SetFrequency(433000000);
	rx_modem->ChangeState(TUNE_RX);
	rx_modem->ChangeState(SPI_ACTIVE);
	rx_modem->ClearInterrupts(); // OK!

	unsigned char PART_INFO[8];
	if(!rx_modem->PartInfo(PART_INFO))
	{
		DBG("RX modem NOP failed!");
	}
	DBG("RX Part info = %X", PART_INFO[1]*256 + PART_INFO[2]);
	rx_modem->StartRX(64, &SyncInterrupt);

	while(1)
	{
		rx_modem->PartInfo(PART_INFO);
		if(PART_INFO[1] == 0x44 && PART_INFO[2] == 0x63)
			modem_status->status = 1;
		else
			modem_status->status = 0;

		rx_modem->Temperature(modem_status->temperature);

		vTaskDelay(1000);
	}
}

void SyncInterrupt()
{
	LED_RED = !LED_RED;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(BINARY_MODEM, &xHigherPriorityTaskWoken);
	if (xHigherPriorityTaskWoken != pdFALSE)
	{
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
}

