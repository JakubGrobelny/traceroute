CC = gcc
STD = -std=c11
FLAGS = -Wall -Wextra -Wpedantic --pedantic -pedantic-errors -fsanitize=address,undefined

TARGET = traceroute

SRC = $(shell find . -name "*.c")
OBJ = $(patsubst %.c, %.o, $(SRC))

$(TARGET) : $(OBJ)
	$(CC) $(STD) $(FLAGS) -o $(TARGET) $(OBJ)

%.o : %.c
	$(CC) $(FLAGS) -MMD -c $< -o $@

clean:
	$(RM) *.o
	$(RM) *.d

distclean:
	$(RM) *.o
	$(RM) *.d
	$(RM) $(TARGET)
