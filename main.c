#include <avr/io.h>
#include <util/delay.h>

#include <FreeRTOS.h>
#include <task.h>

static void blink(void* vParams);
void vApplicationTickHook(void);

int main(void)
{
	PORTB.DIRSET = (1<<3)|(1<<7);
	PORTB.OUTSET = (1<<7);

	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_2X_gc | ~CLKCTRL_PEN_bm);

	xTaskCreate(blink, "Blink", 120, NULL, tskIDLE_PRIORITY+1, NULL);

	vTaskStartScheduler();

	for(;;)
	{
		PORTB.OUTTGL = (1<<7);
		_delay_ms(250);
	}

	return 1;
}

void blink(void* vParams)
{
	for(;;)
	{
		PORTB.OUTTGL = (1<<3);
		vTaskDelay(203);
	}
}

void vApplicationTickHook(void)
{
	// PORTB.OUTTGL = (1<<3);
}
