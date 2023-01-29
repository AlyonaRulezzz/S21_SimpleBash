.PHONY: all re s21_cat s21_cat.o clean cpp_lint_h cpp_lint_c cppcheck
#.PHONY если есть файлы с именем как цель, они игнорируются
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -ggdb -D_GNU_SOURCE
# -ggdb для дебаггинга (вместо g)
# -lm и -lpthread для линковки
# -lrt для компиляции библиотек librt.a или librt.so
# -lsubunit для check.h
TST_LIBS := -lm -lpthread
OS := $(shell uname -s)

ifeq ($(OS),Linux)
	CFLAGS+=-D LINUX
else
	ifeq ($(OS),Darwin)
		CFLAGS+=-D MAC
	endif
endif

all : s21_cat

re : clean s21_cat

s21_cat: s21_cat.o
	@$(CC) $(CFLAGS) s21_cat.o $(TST_LIBS) -o s21_cat
	@sh t_cat.sh

s21_cat.o : s21_cat.c
	@$(CC) $(CFLAGS) -c s21_cat.c -o s21_cat.o

clean :
	@rm -rf *.o s21_cat s21_cat.d* log.txt

cpplint :
	@cp ../../materials/linters/CPPLINT.cfg .
	@python3 ../../materials/linters/cpplint.py *.h *.c
#	@rm -f ../../src/cat/CPPLINT.cfg

cppcheck :
	cppcheck s21_cat.c

valgrind:
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./s21_cat -b text.txt
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./s21_cat -e text.txt
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./s21_cat -n text.txt
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./s21_cat -s text.txt
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./s21_cat -t text.txt
	valgrind --leak-check=full --track-origins=yes --trace-children=yes -s ./s21_cat -v text.txt

leaks:
	leaks -atExit -- ./s21_cat -b text.txt
	leaks -atExit -- ./s21_cat -e text.txt
	leaks -atExit -- ./s21_cat -n text.txt
	leaks -atExit -- ./s21_cat -s text.txt
	leaks -atExit -- ./s21_cat -t text.txt
	leaks -atExit -- ./s21_cat -v text.txt