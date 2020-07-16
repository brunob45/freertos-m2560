#if !defined(HARDWARE_H)
#define HARDWARE_H

#include <avr/io.h>

#if defined(DDRB)
#define LED_PORT PORTB
#define LED_DIR DDRB
#define LED_MASK (1<<7)

#define HARDWARE_INIT() ;

#else
#define LED_PORT PORTB.OUT
#define LED_DIR PORTB.DIR
#define LED_MASK (1<<7)

#define HARDWARE_INIT() {\
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_2X_gc | ~CLKCTRL_PEN_bm);\
}

#endif


#endif // HARDWARE_H