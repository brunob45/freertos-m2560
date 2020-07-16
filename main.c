#include <avr/io.h>
#include <util/delay.h>

#include <FreeRTOS.h>
#include <task.h>

#include "hardware.h"

static void blink(void* vParams);
void vApplicationTickHook(void);

int main(void)
{
	LED_DIR = (1<<3)|LED_MASK;
	LED_PORT = LED_MASK;

	HARDWARE_INIT();

	xTaskCreate(blink, "Blink", 120, NULL, tskIDLE_PRIORITY+1, NULL);

	vTaskStartScheduler();

	for(;;)
	{
		LED_PORT ^= LED_MASK;
		_delay_ms(250);
	}

	return 1;
}

void blink(void* vParams)
{
	for(;;)
	{
		LED_PORT ^= LED_MASK;
		vTaskDelay(203);
	}
}

void vApplicationTickHook(void)
{
	LED_PORT ^= (1<<3);
}
