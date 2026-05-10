CC=gcc
CFLAGS=-Wall -Wextra -Iinclude

SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

TARGET=monitor

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean:
	rm -f src/*.o $(TARGET)