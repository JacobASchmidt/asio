CC=gcc
FLAGS=-O3 -Wall -Wextra
SRC=src
OBJ=obj
ASM=asm
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
ASMS=$(patsubst $(SRC)/%.c, $(ASM)/%.s, $(SRCS))
BINDIR=lib
BIN=$(BINDIR)/libjasio.a

all: $(BIN) asm


$(BIN): $(OBJS) $(ASMS)
	ar rcs $(BIN) $(OBJS)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(FLAGS) -c $< -o $@

$(ASM)/%.s: $(SRC)/%.c
	$(CC) $(FLAGS) -c -S $< -o $@

clean:
	rm -r $(BINDIR)/* $(OBJ)/* $(ASM)/* 

run: $(BIN)
	./$(BIN)
