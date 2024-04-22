
all: build create-obj

create-obj: main.objdump
build: main.out

main.objdump: main.out
	objdump -D main.out > main.objdump

main.out: main.c.o
	cc -o $@ $<

main.c.o: src/main.c
	cc -c -o $@ -O0 -g -Wpedantic -Wall $<

clean:
	rm -f main.out
	rm -f main.objdump
	rm -f main.c.o
