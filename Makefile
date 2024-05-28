.PHONY:all clean run
run:all
	./main
debug:all
	gdb ./main
all:main
main:src/main.c
	g++ src/main.c -I include/ -o main -g
clean:
	-rm main