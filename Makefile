TARGET = main
CC = clang++
CCFLAGS = -std=c++14 -Wall -Wextra

SFML_INCLUDE = -I./include/
SFML_LIB = -L./lib/ -lsfml-graphics -lsfml-window -lsfml-system

PREFIX_SRC = ./src/
PREFIX_OBJ = ./obj/

SRC = $(wildcard $(PREFIX_SRC)*.cpp)
OBJ = $(patsubst $(PREFIX_SRC)%.cpp, $(PREFIX_OBJ)%.o, $(SRC))

$(TARGET) : $(OBJ)
	$(CC) $(CCFLAGS) $(SFML_INCLUDE) -o $(TARGET) $(OBJ) $(SFML_LIB)

$(PREFIX_OBJ)%.o : $(PREFIX_SRC)%.cpp
	$(CC) $(CCFLAGS) $(SFML_INCLUDE) -c $< -o $@

clean :
	rm $(TARGET) $(PREFIX_OBJ)*.o

open : $(TARGET)
	./$(TARGET)
