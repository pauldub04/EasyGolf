
.PHONY: all clean

all: run

run: compile
	./main

compile:
	gcc -o main main.c -Wall -std=c99 -D_DEFAULT_SOURCE \
	-Wno-missing-braces -Wunused-result -O2 -I/usr/local/include -L/usr/local/lib  \
	-lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -latomic -DPLATFORM_DESKTOP