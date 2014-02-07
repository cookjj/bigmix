all:
	gcc *.c -O2 -Wall -o bigmix

install:
	cp bigmix ~/bin/bigmix

