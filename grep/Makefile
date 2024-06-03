CC = gcc -std=c11
CFLAGS = -Wall -Werror -Wextra -D_POSIX_C_SOURCE=201710L
GREP = s21_grep

all: format-check build

build: s21_grep

rebuild: clean build

format-check:
	clang-format -n --style=Google *.c *.h

format:
	clang-format -i --style=Google *.c *.h

s21_grep: $(GREP).o
	$(CC) $(CFLAGS) $^ -o $(GREP)
	rm $(GREP).o

$(GREP).o: $(GREP).c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(GREP)