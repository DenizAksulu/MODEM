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
	DBG("RX task start.");

	SPI tx_modem_spi(PA_7, PA_6, PA_5);				// SPI for RX modem
	DigitalOut tx_modem_nsel(PA_2, 1);					// NSEL for RX modem
	DigitalOut tx_modem_sdn(PE_5, 1);					// SDN for RX modem
	Si4463 tx_modem(&tx_modem_spi, &tx_modem_nsel, &tx_modem_sdn);

	DBG("Power up TX modem...");
	if(!tx_modem.PowerUp())
		DBG("Power up failed!");
	DBG("Power up successful!")
	if(!tx_modem.NOP())
		DBG("TX modem NOP failed!");
	DBG("TX modem NOP successful!");

	unsigned char PART_INFO[8];
	if(!tx_modem.PartInfo(PART_INFO))
		DBG("TX modem NOP failed!");
	DBG("TX Part info = %X", PART_INFO[1]*256 + PART_INFO[2]);

	while(1)
	{
		double temperature = 0;
		if(!tx_modem.Temperature(&temperature))
			DBG("TX modem temperature failed!");
		DBG("TX modem temperature = %f", temperature);
		vTaskDelay(1000);
	}
}



#endif /* TXTASK_CPP_ */
