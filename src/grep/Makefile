CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11 -pedantic

all: s21_grep

s21_grep: s21_grep.c
	$(CC) $(FLAGS) s21_grep.c -o s21_grep

test: 
	bash tests.sh

check:
	clang-format -style=Google -n *.c *.h

style:
	clang-format -style=Google -i *.c *.h

clean:
	rm -f s21_grep
