CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
TARGET = bankers
SRC = bankers.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -static -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o *~
