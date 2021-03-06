#include "tft.h"
#include "commands.h"

#include <avr/io.h>
#include <avr/pgmspace.h>

#include <spi.h>

#include <FreeRTOS.h>
#include <task.h>

SPIClass spi;

static const uint8_t PROGMEM initcmd[] = {
  0xEF, 3, 0x03, 0x80, 0x02,
  0xCF, 3, 0x00, 0xC1, 0x30,
  0xED, 4, 0x64, 0x03, 0x12, 0x81,
  0xE8, 3, 0x85, 0x00, 0x78,
  0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
  0xF7, 1, 0x20,
  0xEA, 2, 0x00, 0x00,
  ILI9341_PWCTR1  , 1, 0x23,             // Power control VRH[5:0]
  ILI9341_PWCTR2  , 1, 0x10,             // Power control SAP[2:0];BT[3:0]
  ILI9341_VMCTR1  , 2, 0x3e, 0x28,       // VCM control
  ILI9341_VMCTR2  , 1, 0x86,             // VCM control2
  ILI9341_MADCTL  , 1, 0x48,             // Memory Access Control
  ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
  ILI9341_PIXFMT  , 1, 0x55,
  ILI9341_FRMCTR1 , 2, 0x00, 0x18,
  ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27, // Display Function Control
  0xF2, 1, 0x00,                         // 3Gamma Function Disable
  ILI9341_GAMMASET , 1, 0x01,             // Gamma curve selected
  ILI9341_GMCTRP1 , 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
    0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
  ILI9341_GMCTRN1 , 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
    0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
  ILI9341_SLPOUT  , 0x80,                // Exit Sleep
  ILI9341_DISPON  , 0x80,                // Display on
  0x00                                   // End of list
};

static void sendCommand(uint8_t command, uint8_t* data = nullptr, uint8_t size = 0)
{
    spi.begin_tx();
    PORTC.OUTCLR = PIN5_bm;
    spi.send(command);
    PORTC.OUTSET = PIN5_bm;
    spi.send(data, size);
    spi.end_tx();
}

static void sendCommand(uint8_t command, const uint8_t* data, uint8_t size)
{
    spi.begin_tx();
    PORTC.OUTCLR = PIN5_bm;
    spi.send(command);
    PORTC.OUTSET = PIN5_bm;
    while(size--)
    {
        spi.send(pgm_read_byte(data++));
    }
    spi.end_tx();
}

TFTClass::TFTClass()
{
    PORTC.DIRSET = PIN5_bm;
    PORTC.OUTSET = PIN5_bm;
}

void TFTClass::begin()
{
    spi.begin();

    sendCommand(ILI9341_SWRESET);
    vTaskDelay(152);

    uint8_t cmd, x, numArgs;
    const uint8_t* addr = initcmd;
    while((cmd = pgm_read_byte(addr++)) > 0)
    {
        x = pgm_read_byte(addr++);
        numArgs = x & 0x7F;
        sendCommand(cmd, addr, numArgs);
        addr += numArgs;
        if(x & 0x80)
        {
            vTaskDelay(152);
        }
    }
}

void TFTClass::invertDisplay(bool invert)
{
    sendCommand(invert ? ILI9341_INVON : ILI9341_INVOFF);
}