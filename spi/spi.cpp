#include "spi.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <FreeRTOS.h>
#include <task.h>

static volatile TaskHandle_t xTaskToNotify = nullptr;

ISR(SPI0_INT_vect)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR( xTaskToNotify, &xHigherPriorityTaskWoken );
    xTaskToNotify = nullptr;
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void SPIClass::begin()
{
    PORTMUX.CTRLB |= PORTMUX_SPI0_ALTERNATE_gc;

    PORTC.DIRSET |= PIN3_bm | PIN2_bm | PIN0_bm;
    PORTC.OUTSET |= PIN3_bm;

    SPI0.CTRLA |= SPI_MASTER_bm;

    SPI0.CTRLA |= SPI_PRESC_DIV4_gc;
    SPI0.CTRLA |= SPI_CLK2X_bm;

    SPI0.CTRLB |= SPI_BUFEN_bm; // Buffer Mode Enable

    SPI0.CTRLB |= SPI_SSD_bm;

    SPI0.INTCTRL |= SPI_RXCIE_bm;

    SPI0.CTRLA |= SPI_ENABLE_bm;
}

void SPIClass::begin_tx()
{
    PORTC.OUTCLR = PIN3_bm;
}

void SPIClass::end_tx()
{
    PORTC.OUTSET = PIN3_bm;
}

uint8_t SPIClass::recv()
{
    return tx(0);
}

void SPIClass::send(uint8_t data)
{
    tx(data);
}
void SPIClass::send(uint8_t* data, uint8_t size)
{
    while(size--)
    {
        tx(data[size]);
    }
}

uint8_t SPIClass::tx(uint8_t data)
{
    xTaskToNotify = xTaskGetCurrentTaskHandle();

    SPI0.DATA = data;

    ulTaskNotifyTake( pdTRUE, 5 );

    return SPI0.DATA;
}