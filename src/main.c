
#include <avr/io.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

static void blink(void*);

/*
 * The idle hook is used to scheduler co-routines.
 */
void vApplicationIdleHook(void);

/*-----------------------------------------------------------*/

int main(void)
{
    xTaskCreate(blink, "Blink", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    vTaskStartScheduler();

    return 0;
}
/*-----------------------------------------------------------*/

static void blink(void* pvParameters)
{
    (void)pvParameters;

    DDRB = (1 << 7);

    while (1)
    {
        vTaskDelay(1000);
        PORTB ^= (1<<7);
    }
}

void vApplicationIdleHook(void)
{
}
