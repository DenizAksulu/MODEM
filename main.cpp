#include "MODEM.h"
#include "RXTask.h"

void WDTTaskCode(void* Parameters);

using namespace MODEM;

int main()
{
	MUTEX_DEBUG = xSemaphoreCreateMutex();
#if DEBUG
	DebugPort.baud(115200); // Configure debug port.
#endif
	time_t currentTime;
	if(time(&currentTime) <= 0) set_time(0); // In case of RTC error, set time to 0.

	DBG("OBCOMMS Modem code started with system core clock: %d Hz", SystemCoreClock);

	// ***************************************************************************************
	// Define tasks **************************************************************************
	xTaskCreate(WDTTaskCode, "WDTTask", 512, NULL, (tskIDLE_PRIORITY + 11UL), &WDTTaskHandle);
	xTaskCreate(RXTaskCode, "RXTask", 512, NULL, (tskIDLE_PRIORITY + 5UL), &RXTaskHandle);
	// ***************************************************************************************
	// ***************************************************************************************

	// ***************************************************************************************
	// Start task scheduler ******************************************************************
	vTaskStartScheduler();
    while(1)
    {
    	// Code will never enter this loop...
    }
	// ***************************************************************************************
	// ***************************************************************************************
}

void WDTTaskCode(void* Parameters)
{
	DBG("WDT task start.");
	while(1)
	{
		WDI = !WDI;
		LED_BLUE = !LED_BLUE;
		vTaskDelay(1000);
	}
}
