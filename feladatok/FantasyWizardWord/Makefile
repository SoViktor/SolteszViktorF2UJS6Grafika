# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -IC:/vcpkg-master/installed/x64-windows/include -IC:/vcpkg-master/installed/x64-windows/include/SDL2
LDFLAGS = -LC:/vcpkg-master/installed/x64-windows/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lGLEW32 -lopengl32 -lm


# Source and output
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
EXE = wizard_game

# Default target
all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(LDFLAGS)

# Build with debug info
debug: CFLAGS += -g
debug: clean $(EXE)

# Clean object files and executable
clean:
	rm -f src/*.o $(EXE)

# Dependencies
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
