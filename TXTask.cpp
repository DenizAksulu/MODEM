/*
 * TXTask.cpp
 *
 *  Created on: 31 Oca 2017
 *      Author: Mehmet Deniz Aksulu
 */

#ifndef TXTASK_CPP_
#define TXTASK_CPP_

#include "TXTask.h"
#include "Si4463.h"

using namespace MODEM;

void TXTaskCode(void* Parameters)
{
	DBG("TX task start.");
	STRUCT_MODEM_STATUS* modem_status = new STRUCT_MODEM_STATUS;
	xQueueSend(QUEUE_TX_STATUS, &modem_status, portMAX_DELAY);

	SPI* tx_modem_spi = new SPI(PA_7, PA_6, PA_5);					// SPI for TX modem
	DigitalOut* tx_modem_nsel = new DigitalOut(PA_2, 1);					// NSEL for TX modem
	DigitalOut* tx_modem_sdn = new DigitalOut(PE_5, 1);					// SDN for TX modem
	Si4463* tx_modem = new Si4463(tx_modem_spi, tx_modem_nsel, tx_modem_sdn);
	xQueueSend(QUEUE_TX_OBJECT, &tx_modem, portMAX_DELAY);					// Send rx modem object

	if(!tx_modem->PowerUp())
	{
		DBG("Power up failed.");
	}

	if(!tx_modem->SetTXModem())
	{
		DBG("TX modem initialization failed.");
	}

	if(!tx_modem->SetPA(0x0F))
	{
		DBG("TX modem set PA failed.");
	}

	if(!tx_modem->SetFRR(PACKET_HANDLER_PENDING, PACKET_HANDLER_STATUS, MODEM_PENDING, CHIP_PENDING))
	{
		DBG("TX modem set FRR failed.");
	}

	unsigned char PART_INFO[8];
	if(!tx_modem->PartInfo(PART_INFO))
	{
		DBG("TX modem NOP failed!");
	}
	DBG("TX Part info = %X", PART_INFO[1]*256 + PART_INFO[2]);

	if(!tx_modem->SetFrequency(430000000))
	{
		DBG("TX modem frequency set failed.");
	}
	DBG("TX modem frequency set to %d Hz.", 430000000);

	double temperature = 0;
	if(!tx_modem->Temperature(&temperature))
	{
		DBG("TX modem temperature failed.");
	}

	if(!tx_modem->ChangeState(TUNE_TX))
	{
		DBG("TX modem state transition failed: %d", TUNE_TX);
	}

	vTaskDelay(300);

	if(!tx_modem->ChangeState(SPI_ACTIVE))
	{
		DBG("TX modem state transition failed: %d", SPI_ACTIVE);
	}

	if(!tx_modem->ChangeState(TX))
	{
		DBG("TX modem state transition failed: %d", TX);
	}

	if(!tx_modem->ClearInterrupts())
	{
		DBG("Clear interrupts failed.");
	}
	double temp = 0;
	unsigned char buf[256];
	for(int i = 0; i < 256; i++)
	{
		buf[i] = i;
	}
	while(1)
	{
		tx_modem->PartInfo(PART_INFO);
		if(PART_INFO[1] == 0x44 && PART_INFO[2] == 0x63)
			modem_status->status = 1;
		else
			modem_status->status = 0;

		tx_modem->Temperature(&temp);
		DBG("TX Modem temperature %f", temp);

		tx_modem->SendData(buf, 256);
		vTaskDelay(1000);
	}
}



#endif /* TXTASK_CPP_ */
