CC = gcc
STD = -std=gnu11
FLAGS = -Wall -Wextra -Wpedantic --pedantic -pedantic-errors 
DEBUG_FLAGS = -fsanitize=address,undefined -O0 -g -DDEBUG

TARGET = traceroute

SRC = $(shell find . -name "*.c")
OBJ = $(patsubst %.c, %.o, $(SRC))

$(TARGET) : $(OBJ)
	$(CC) $(STD) $(FLAGS) -o $(TARGET) $(OBJ)

%.o : %.c
	$(CC) $(FLAGS) -MMD -c $< -o $@

debug: $(TARGET)
debug: FLAGS += $(DEBUG_FLAGS)

clean:
	$(RM) *.o
	$(RM) *.d

distclean:
	$(RM) *.o
	$(RM) *.d
	$(RM) $(TARGET)
