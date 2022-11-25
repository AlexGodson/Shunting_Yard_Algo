CC=gcc -std=c99
INC = /Includes
libs = -lm
CFLAGS=-g -Wall -Werror
_SRC=src
SRC=$(wildcard $(_SRC)/*.c)
_OBJ=src/obj
OBJ=$(patsubst $(_SRC)/%.c, $(_OBJ)/%.o, $(SRC))
OUT=calculator

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(OUT) $(libs)

$(_OBJ)/%.o: $(_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(libs)

run: $(OUT)
	./$(OUT)

clear:
	rm $(OBJ)*
	rm $(OUT)

