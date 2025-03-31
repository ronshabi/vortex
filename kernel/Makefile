CC=aarch64-none-elf-gcc
AS=aarch64-none-elf-as
LD=aarch64-none-elf-ld

CFLAGS  = -std=c99 -g -O0
CFLAGS += -Wall -Wextra -Wformat -Wconversion
CFLAGS += -ffreestanding -nostdlib -fno-builtin -fno-lto
CFLAGS += -mgeneral-regs-only
CFLAGS += -fno-omit-frame-pointer
CFLAGS += -fno-optimize-sibling-calls

LDFLAGS = -nostdlib

ASFLAGS = -g 

OBJECTS = main.o boot.o printk.o uart.o logbuffer.o klibc.o \
          stringutils.o exception_info.o panic.o exception.o halt.o

TARGET = kernel.elf

$(TARGET): linker.ld $(OBJECTS)
	$(LD) $(LDFLAGS) -Tlinker.ld $(OBJECTS) -o $@

boot.o: boot.S
	$(AS) $(ASFLAGS) -o $@ boot.S

halt.o: halt.S
	$(AS) $(ASFLAGS) -o $@ halt.S

exception.o: exception.S
	$(AS) $(ASFLAGS) -o $@ exception.S

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGET)
