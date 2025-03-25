CC = gcc
INCLUDE = -Iinclude
CFLAGS = -Wall -Wextra $(INCLUDE)

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BIN = $(BIN_DIR)/app.exe

$(BIN): $(OBJ) $(BIN_DIR) $(OBJ_DIR)
	$(CC) $(OBJ) -o $(BIN)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/app.exe

run:
	$(BIN)

.PHONY: clean