
CC=avr-gcc
AR=avr-ar

TITLE=freertos

INC=-I. -I../../kernel/include

DIR_PORT=.
DIR_KERNEL=../../kernel
DIR_BUILD=build

SRC_PORT=$(DIR_PORT)/port.c $(DIR_PORT)/heap_1.c
SRC_KERNEL=$(DIR_KERNEL)/list.c $(DIR_KERNEL)/queue.c $(DIR_KERNEL)/tasks.c

SRC=$(SRC_PORT) $(SRC_KERNEL)

OBJ=$(SRC_PORT:$(DIR_PORT)/%.c=$(DIR_BUILD)/%.o) $(SRC_KERNEL:$(DIR_KERNEL)/%.c=$(DIR_BUILD)/%.o)

TARGET=lib$(TITLE).a

CFLAGS=-mmcu=$(MCU) -DF_CPU=$(F_CPU) $(INC) -O2

.PHONY:clean

all:$(TARGET)

$(TARGET):$(OBJ)
	$(AR) r $@ $^

$(DIR_BUILD)/%.o:$(DIR_KERNEL)/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIR_BUILD)/%.o:$(DIR_PORT)/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJ)
