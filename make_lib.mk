CC=avr-gcc
AR=avr-ar

TARGET=lib$(TITLE).a

CFLAGS+=-mmcu=$(MCU) -DF_CPU=$(F_CPU) -O2 -I. -I../freertos/kernel/include -I../freertos/port/$(MCU)

SRC=$(wildcard *.cpp)
INC=$(wildcard *.h)
OBJ=$(patsubst %.cpp,%.o,$(SRC))

.PHONY:clean

all:$(TARGET)

$(TARGET):$(OBJ)
	$(AR) r $@ $^

%.o:%.cpp $(INC)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJ)