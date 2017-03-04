/*
 * MODEM.h
 *
 *  Created on: 24 Oca 2017
 *      Author: Mehmet Deniz Aksulu
 */


#ifndef MODEM_H_
#define MODEM_H_

#include "FreeRTOS.h"
#include "mbed.h"
#include "semphr.h"
#include "task.h"
#include "SDFileSystem.h"


#define DEBUG 1 // Debug mode enabled

#if DEBUG
	#define DBG(...) \
			if(xSemaphoreTake(MUTEX_DEBUG, portMAX_DELAY) == pdTRUE) \
			{ \
				time_t seconds = time(NULL);\
				char buffer[32]; \
    			strftime(buffer, 32, "%Y %b %d    %H:%M:%S", localtime(&seconds)); \
				DebugPort.printf("\n %s | %30s:%-4d | ", buffer, __FILE__, __LINE__); \
				DebugPort.printf(__VA_ARGS__); \
				xSemaphoreGive(MUTEX_DEBUG); \
			}
#else
#define DBG(...)
#endif

namespace MODEM
{
	/*
	 * PORT and GPIO Definitions
	 */
	extern DigitalOut WDI;
	extern DigitalOut LED_BLUE;
	extern DigitalOut LED_RED;
	#if DEBUG
	extern Serial DebugPort;
	#endif


	/*
	 * Task handle definitions
	 */
	extern xTaskHandle WDTTaskHandle;    				// Task handle for WDT task
	extern xTaskHandle RXTaskHandle;
	extern xTaskHandle TXTaskHandle;
	extern xTaskHandle CommandHandlingTaskHandle;
	/*
	 * MUTEX and Semaphore definitions
	 */
	extern SemaphoreHandle_t MUTEX_DEBUG;
	extern SemaphoreHandle_t BINARY_MODEM;
	extern QueueHandle_t QUEUE_RX_OBJECT;
	extern QueueHandle_t QUEUE_TX_OBJECT;
}
#endif /* MODEM_H_ */
