/*
 * MODEM.cpp
 *
 *  Created on: 24 Oca 2017
 *      Author: Mehmet Deniz Aksulu
 */

#include "MODEM.h"

namespace MODEM
{
	/*
	 * PORT and GPIO Definitions
	 */
	DigitalOut WDI(PA_3, 1);
	DigitalOut LED_BLUE(PB_10, 0);
	DigitalOut LED_RED(PB_11, 0);
	DigitalOut SW1(PC_3, 1);
	DigitalOut SW2(PC_1, 0);
	DigitalOut ENABLE_PA(PE_3, 1);
	DigitalOut BOOT_0(PB_4, 0);
    DigitalOut NRESET(PB_6, 0);
	#if DEBUG
	Serial DebugPort(PA_0, PA_1, "DebugPort");
	#endif


	/*
	 * Task handle definitions
	 */
	xTaskHandle WDTTaskHandle = NULL;    				// Task handle for WDT task
	xTaskHandle RXTaskHandle = NULL;    				// Task handle
	xTaskHandle TXTaskHandle = NULL;    				// Task handle
	xTaskHandle CommandHandlingTaskHandle = NULL;    				// Task handle
	xTaskHandle PCHandlingTaskHandle = NULL;    				// Task handle

	/*
	 * MUTEX and Semaphore definitions
	 */
	SemaphoreHandle_t MUTEX_DEBUG = NULL;
	SemaphoreHandle_t BINARY_MODEM = NULL;
	SemaphoreHandle_t BINARY_PC = NULL;
	QueueHandle_t QUEUE_RX_OBJECT = NULL;
	QueueHandle_t QUEUE_TX_OBJECT = NULL;
	QueueHandle_t QUEUE_RX_STATUS = NULL;
	QueueHandle_t QUEUE_TX_STATUS = NULL;
	QueueHandle_t QUEUE_PC_DATA = NULL;
	QueueHandle_t QUEUE_PC_DATA_LENGTH = NULL;

}
