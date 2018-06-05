CC=gcc
# Release
CFLAGS= -Wall -Wextra -std=c99 -O3
# Debug
#CFLAGS= -Wall -Wextra -std=c99 -O0 -g3
# Dev
#CFLAGS= -Wall -Wextra -std=c99 -O0 -g3 #-fno-omit-frame-pointer
#LDFLAGS= -fsanitize=address

SRC= main.c bmp.c stegano.c tool.c
DEP = ${SRC:.c=.d}
OBJ= ${SRC:.c=.o}

-include ${DEP}

all: main img data

main: ${OBJ}

img:
	cp img.bmp tmp.bmp

view:
	feh tmp.bmp&

.PHONY: clean cl test

data:
	dd if=/dev/urandom of=data bs=1 count=4096

test: cl all
	./main -i tmp.bmp -o tmp -f data
	./main -i tmp -o text -d
	diff text data

clean cl:
	rm -rf tmp
	rm -rf tmp.bmp
	rm -rf data
	rm -rf main
	rm -rf text
	rm -rf ${OBJ}
	rm -rf ${DEP}
