#
# makefile with argument
#
TARGET := ${arg1}
HEADER := ${arg2}
SRC := $(TARGET).c
INC := $(HEADER).h
OBJ := $(TARGET).o

CFLAGS := -g -Wall
CC := gcc $(CFLAG)

all: clean $(TARGET)

$(TARGET): $(OBJ) 
	$(CC) -o $@ $^ $(LFLAGS)
$(OBJ): $(SRC) $(INC)
	$(CC) -c $^
clean:
	rm -f $(TARGET) $(OBJ)
