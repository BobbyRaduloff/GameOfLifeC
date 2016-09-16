CC = clang
FILES = main.c
FLAGS = -g -std=c11 -lSDL2 -Werror -Wall
NAME = GameOfLife

all:
    $(CC) $(FILES) $(FLAGS) -o $(NAME)

install:
    cp $(NAME) /usr/bin/$(NAME)

delete:
    rm /usr/bin/$(NAME)
