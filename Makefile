CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -O2 -Iinclude
LDFLAGS = -lm

SRC_DIR = src
INC_DIR = include
DATA_DIR = data
TEST_DIR = tests

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = turne

.PHONY: all run test valgrind clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) < $(DATA_DIR)/entrada.in

test: $(TARGET)
	./$(TARGET) < $(DATA_DIR)/entrada.in > $(DATA_DIR)/meu_resultado.out
	diff -u $(DATA_DIR)/saida.out $(DATA_DIR)/meu_resultado.out

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) < $(DATA_DIR)/entrada.in

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET) $(DATA_DIR)/meu_resultado.out
