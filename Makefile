CC=gcc
ASMBIN=nasm

imgt : image_test.o image.o _code128_generation.o

_code128_generation.o : _code128_generation.s
	$(ASMBIN) -o _code128_generation.o -f elf -g -F dwarf _code128_generation.s

image.o : image.h image.c
	$(CC) -m32 -c -g -O0 image.c

image_test.o : image.h image_test.c
	$(CC) -m32 -c -g -O0 image_test.c

imgt : image_test.o image.o _code128_generation.o
	$(CC) -m32 -g -o imgt image_test.o image.o _code128_generation.o

clean :
	rm *.o
	rm imgt
