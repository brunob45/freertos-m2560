
CC=avr-gcc
LD=avr-gcc
COPY=avr-objcopy
SIZE=avr-size

TITLE=blink

export MCU=attiny1607
export F_CPU=20000000

INC=-I. -Ifreertos/kernel/include -Ifreertos/port/$(MCU)

SRC=main.c

OBJ=$(SRC:.c=.o )
LIB=freertos/port/$(MCU)/libfreertos.a

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

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

freertos/port/$(MCU)/libfreertos.a:
	cd freertos/port/$(MCU) && make -e

clean:
	rm -f $(TARGET) $(EXEC) $(OBJ)

cleanall: clean
	cd freertos/port/$(MCU) && make clean
