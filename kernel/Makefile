CC=aarch64-none-elf-gcc
AS=aarch64-none-elf-as
LD=aarch64-none-elf-ld
PYTHON=python3

CFLAGS  = -std=c99 -g -O0
CFLAGS += -Wall -Wextra -Wformat -Wconversion
CFLAGS += -ffreestanding -nostdlib -fno-builtin -fno-lto
CFLAGS += -mgeneral-regs-only
CFLAGS += -fno-omit-frame-pointer
CFLAGS += -fno-optimize-sibling-calls

LDFLAGS = -nostdlib
LDSCRIPT = linker.ld

ASFLAGS = -g

OBJECTS = main.o boot.o printk.o uart.o logbuffer.o klibc.o \
          stringutils.o exception_info.o panic.o exception.o halt.o \
		  ksyms.o


TARGET = kernel.elf

GENERATED = symbols.map symbols.S

$(TARGET): linker.ld $(OBJECTS)
	$(LD) $(LDFLAGS) -T $(LDSCRIPT) $(OBJECTS) -o $@

	@# Generate kernel symbols
	$(SHELL) ../scripts/gen_symbols_map.sh $(TARGET) > symbols.map
	$(PYTHON) ../scripts/gen_symbols_asm.py symbols.map > symbols.S

	@# Compile the symbols to an object
	$(AS) $(ASFLAGS) -o symbols.o symbols.S

	@# Link again!
	$(LD) $(LDFLAGS) -T $(LDSCRIPT) $(OBJECTS) symbols.o -o $@

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
	rm -f $(GENERATED)