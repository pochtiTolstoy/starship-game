#OBJS = src/main.cpp src/texture/LTexture.cpp src/entity/ship.cpp src/util/render_pipe.cpp
#OBJ_NAME = APP
#CC = g++
#LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

#all: $(OBJS)
#	$(CC) $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)

#debug: $(OBJS)
#	$(CC) -g $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)

SRC_DIRS := src src/texture src/entity src/util src/game_process
SRCS := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
OBJS := $(SRCS:.cpp=.o)
OBJ_NAME := APP
CC := g++

LINKER_FLAGS := -lSDL2 -lSDL2_image -lSDL2_ttf

DEBUG_FLAGS :=

all: $(OBJS)
	$(CC) $(SRCS) $(LINKER_FLAGS) -o $(OBJ_NAME)

debug: $(SRCS)
	$(CC) $(DEBUG_FLAGS) $(SRCS) $(LINKER_FLAGS) -o $(OBJ_NAME)
