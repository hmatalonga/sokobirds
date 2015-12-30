OBJS=main.c draw.c helper.c
CC=gcc
COMPILER_FLAGS=-Wall -Werror -g
LINKER_FLAGS=-lSDL2 -lSDL2_mixer -lGL
DEPS=sokobirds.h
OBJ_NAME=sokobirds

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(COMPILER_FLAGS)
sokobirds: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
clean:
	rm $(OBJ_NAME)