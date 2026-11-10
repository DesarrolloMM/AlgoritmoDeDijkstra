CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -Wpedantic -g
TARGET  = simulador_rutas
OBJS    = main.o grafo.o dijkstra.o enrutamiento.o tests.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

test: all
	./$(TARGET) --test

clean:
	rm -f $(OBJS) $(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET)

.PHONY: all run test clean