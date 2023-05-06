CC := gcc
SRC_DIR := src
OBJ_DIR := obj
OBJ := $(OBJ_DIR)/main.o $(OBJ_DIR)/mymalloc.o $(OBJ_DIR)/mylist.o
SRC := $(SRC_DIR)/*.c
FLAGS := -pthread -g
main: $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o main
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o