CC = clang
FILES = main.c
FLAGS = -g -std=c11 -lSDL2 -Werror -Wall
NAME = GameOfLife

all:
	$(CC) $(FILES) $(FLAGS) -o $(NAME)

clean:
	rm *.o $(NAME)
