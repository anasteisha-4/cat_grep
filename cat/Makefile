CC = gcc -std=c11
CFLAGS = -Wall -Werror -Wextra
CAT = s21_cat

all: format-check build

build: s21_cat

rebuild: clean build

format-check:
	clang-format -n --style=Google *.c *.h

format:
	clang-format -i --style=Google *.c *.h

s21_cat: $(CAT).o
	$(CC) $(CFLAGS) $^ -o $(CAT)
	rm $(CAT).o

$(CAT).o: $(CAT).c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(CAT)