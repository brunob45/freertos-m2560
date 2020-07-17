#if !defined(SPI_H)
#define SPI_H

#include <stdint.h>

class SPIClass
{
    uint8_t tx(uint8_t);

public:
    void begin();
    void begin_tx();
    void end_tx();
    uint8_t recv();
    void send(uint8_t);
    void send(uint8_t*, uint8_t);
};

#endif // SPI_H