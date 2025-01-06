CC = gcc
CFLAGS = -Iinclude -Wall -g
SRC = src/main.c src/record.c src/block.c src/sequential_file.c src/persistence.c
OBJ = $(SRC:.c=.o)
EXEC = sequential_file

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
