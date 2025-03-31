CC=aarch64-none-elf-gcc
AS=aarch64-none-elf-as
LD=aarch64-none-elf-ld

CFLAGS  = -std=c99 -g -O0
CFLAGS += -Wall -Wextra -Wformat -Wconversion
CFLAGS += -ffreestanding -nostdlib -fno-builtin -fno-lto
CFLAGS += -mgeneral-regs-only

LDFLAGS = -nostdlib

OBJECTS = main.o boot.o printk.o uart.o logbuffer.o klibc.o stringutils.o exception_context.o panic.o exception.o halt.o

TARGET = kernel.elf

$(TARGET): linker.ld $(OBJECTS)
	$(LD) $(LDFLAGS) -Tlinker.ld $(OBJECTS) -o $@

boot.o: boot.S
	$(AS) -o $@ boot.S

halt.o: halt.S
	$(AS) -o $@ halt.S

exception.o: exception.S asmbridge.h
	$(AS) -o $@ exception.S

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGET)
