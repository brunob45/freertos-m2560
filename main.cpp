#include <avr/io.h>
#include <util/delay.h>

#include <FreeRTOS.h>
#include <task.h>

#include "hardware.h"
#include "tft.h"

static void blink(void* vParams);
static void spi_send(void* vParams);

int main(void)
{
	LED_DIR = (1<<3)|LED_MASK;
	LED_PORT = LED_MASK;

	HARDWARE_INIT();

	xTaskCreate(blink, "Blink", 120, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(spi_send, "SPI", 120, NULL, tskIDLE_PRIORITY+2, NULL);

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

static void spi_send(void* vParams)
{
	bool inv = false;

	TFTClass tft;

	tft.begin();

	for(;;)
	{
		vTaskDelay(500);
		tft.invertDisplay(inv);
		inv = !inv;
	}
}

extern "C" void vApplicationTickHook(void)
{
	LED_PORT ^= (1<<3);
}
