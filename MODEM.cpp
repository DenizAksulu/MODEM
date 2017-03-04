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

	/*
	 * MUTEX and Semaphore definitions
	 */
	SemaphoreHandle_t MUTEX_DEBUG = NULL;
	SemaphoreHandle_t BINARY_MODEM = NULL;
	QueueHandle_t QUEUE_RX_OBJECT = NULL;
	QueueHandle_t QUEUE_TX_OBJECT = NULL;
}
