CC = gcc
CFLAGS = -Wall -Werror -Wextra
FLAG = --coverage
OS := $(shell uname -s)

all: clean s21_matrix.a test

s21_matrix.o:
	$(CC) $(CFLAGS) -c s21_matrix.c -o s21_matrix.o

s21_matrix.a: s21_matrix.o
	ar rcs s21_matrix.a s21_matrix.o
	cp s21_matrix.a libs21_matrix
	cp s21_matrix.a ls21_matrix

test: s21_matrix.a
ifeq ($(OS), Darwin)
	$(CC) -c test.c -o test.o
	$(CC) test.o s21_matrix.a -o test -lcheck
	./test
else
	$(CC) test.c s21_matrix.a -o test -lcheck -lsubunit -lrt -lpthread -lm
	./test
endif

gcov:
ifeq ($(OS), Darwin)
	$(CC) $(FLAG) s21_matrix.c test.c -o test -lcheck
else
	$(CC) $(FLAG) s21_matrix.c test.c -o test -lcheck -lsubunit -lrt -lpthread -lm
endif

gcov_report: clean gcov
	./test
	lcov -t “test” -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html

clean:
	rm -rf *.o  *.gc* report *.a test libs21_matrix ls21_matrix test.info

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h
	CK_FORK=no leaks --atExit -- ./test
