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

	SPI rx_modem_spi(PC_12, PC_11, PC_10);				// SPI for RX modem
	DigitalOut rx_modem_nsel(PC_9, 0);					// NSEL for RX modem
	DigitalOut rx_modem_sdn(PD_13, 0);					// SDN for RX modem
	Si4463 rx_modem(&rx_modem_spi, &rx_modem_nsel, &rx_modem_sdn);

	DBG("Power up RX modem...");
	if(!rx_modem.PowerUp())
		DBG("Power up failed!");
	DBG("Power up successful!")
	if(!rx_modem.NOP())
		DBG("RX modem NOP failed!");
	DBG("RX modem NOP successful!");

	unsigned char PART_INFO[8];
	if(!rx_modem.PartInfo(PART_INFO))
		DBG("RX modem NOP failed!");
	DBG("Part info = %X", PART_INFO[1]*256 + PART_INFO[2]);

	while(1)
	{
		double temperature = 0;
		if(!rx_modem.Temperature(&temperature))
			DBG("RX modem temperature failed!");
		DBG("RX modem temperature = %f", temperature);
		vTaskDelay(1000);
	}
}


