CC=aarch64-none-elf-gcc
AS=aarch64-none-elf-as
LD=aarch64-none-elf-ld

CFLAGS  = -std=c99 -g -O0
CFLAGS += -Wall -Wextra -Wformat -Wconversion
CFLAGS += -ffreestanding -nostdlib -fno-builtin -fno-lto
CFLAGS += -mgeneral-regs-only

LDFLAGS = -nostdlib

OBJECTS = main.o boot.o

TARGET = kernel.elf

$(TARGET): linker.ld $(OBJECTS)
	$(LD) $(LDFLAGS) -Tlinker.ld $(OBJECTS) -o $@

boot.o: boot.S
	$(AS) -o $@ $^

main.o: main.c types.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGET)
