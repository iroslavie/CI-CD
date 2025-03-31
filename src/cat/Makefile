CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11 -pedantic

all: s21_cat

s21_cat: Makefile s21_cat.c
	$(CC) $(FLAGS) s21_cat.c -o s21_cat

test: 
	bash tests.sh

check:
	clang-format -style=Google -n *.c *.h

style:
	clang-format -style=Google -i *.c *.h


clean:
	rm -f s21_cat