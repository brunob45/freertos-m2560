/*
 * FreeRTOS Kernel V10.3.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 *
 * Main. c also creates a task called "Check".  This only executes every three
 * seconds but has the highest priority so is guaranteed to get processor time.
 * Its main function is to check that all the other tasks are still operational.
 * Each task that does not flash an LED maintains a unique count that is
 * incremented each time the task successfully completes its function.  Should
 * any error occur within such a task the count is permanently halted.  The
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have
 * changed all the tasks are still executing error free, and the check task
 * toggles an LED.  Should any task contain an error at any time the LED toggle
 * will stop.
 *
 * The LED flash and communications test tasks do not maintain a count.
 */

/*
Changes from V1.2.0

    + Changed the baud rate for the serial test from 19200 to 57600.

Changes from V1.2.3

    + The integer and comtest tasks are now used when the cooperative scheduler
      is being used.  Previously they were only used with the preemptive
      scheduler.

Changes from V1.2.5

    + Set the baud rate to 38400.  This has a smaller error percentage with an
      8MHz clock (according to the manual).

Changes from V2.0.0

    + Delay periods are now specified using variables and constants of
      TickType_t rather than unsigned long.

Changes from V2.6.1

    + The IAR and WinAVR AVR ports are now maintained separately.

Changes from V4.0.5

    + Modified to demonstrate the use of co-routines.

*/

#include <stdlib.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>

#ifdef GCC_MEGA_AVR
/* EEPROM routines used only with the WinAVR compiler. */
#include <avr/eeprom.h>
#endif

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
