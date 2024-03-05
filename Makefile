OBJS = main.cpp LTexture.cpp
OBJ_NAME = APP
CC = g++
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

all: $(OBJS)
	$(CC) $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)

debug: $(OBJS)
	$(CC) -g $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)
