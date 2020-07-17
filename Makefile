
CC=avr-gcc
LD=avr-gcc
COPY=avr-objcopy
SIZE=avr-size

TITLE=blink

export MCU=attiny1607
export F_CPU=20000000

INC=-I. -Ifreertos/kernel/include -Ifreertos/port/$(MCU) -Itft

SRC=main.cpp

OBJ=$(SRC:.cpp=.o )
LIB=freertos/port/$(MCU)/libfreertos.a spi/libspi.a tft/libtft.a

CFLAGS=-mmcu=$(MCU) -DF_CPU=$(F_CPU) $(INC) -O2
LDFLAGS=-mmcu=$(MCU)

TARGET=$(TITLE).elf
EXEC=$(TITLE).hex

.PHONY:clean cleanall hex size install

all:$(TARGET)

hex:$(EXEC)

install:$(EXEC)
	cp $< /run/media/$(USER)/CURIOSITY/

size:$(TARGET)
	$(SIZE) --format=avr --mcu=$(MCU) $<

$(EXEC):$(TARGET)
	$(COPY) -R .eeprom -O ihex $< "$@"

$(TARGET):$(OBJ) $(LIB)
	$(CC) $(LDFLAGS) -o $@ $^

%.o:%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

freertos/port/$(MCU)/libfreertos.a:
	cd freertos/port/$(MCU) && make -e

spi/libspi.a:
	cd spi && make -e

tft/libtft.a:
	cd tft && make -e

clean:
	rm -f $(TARGET) $(EXEC) $(OBJ)

cleanall: clean
	cd freertos/port/$(MCU) && make clean
	cd spi && make clean
	cd tft && make clean
