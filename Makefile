PREFIX := aarch64-none-elf
CC := $(PREFIX)-gcc
AS := $(PREFIX)-as
LD := $(PREFIX)-ld
NM := $(PREFIX)-nm
OBJDUMP := $(PREFIX)-objdump

PYTHON := python3

CFLAGS  = -std=c99 -g -O0
CFLAGS += -Wall -Wextra -Wformat -Wconversion
CFLAGS += -ffreestanding -nostdlib -fno-builtin -fno-lto
CFLAGS += -mgeneral-regs-only
CFLAGS += -fno-omit-frame-pointer
CFLAGS += -fno-optimize-sibling-calls

LDFLAGS = -nostdlib
LDSCRIPT = kernel/linker.ld

ASFLAGS = -g

OBJECTS = main.o boot.o printk.o uart.o logbuffer.o klibc.o \
          stringutils.o exception_info.o panic.o exception.o halt.o \
		  ksyms.o memory.o gicv3.o

TARGET = kernel.elf

BUILD_DIR := build


# -- Append the build directory prefix to artifacts
OBJECTS   := $(foreach o, $(OBJECTS), $(BUILD_DIR)/$(o))
TARGET    := $(BUILD_DIR)/$(TARGET)

.PHONY: all
all: $(TARGET)
	@mkdir -p $(BUILD_DIR)

$(TARGET): $(LDSCRIPT) $(OBJECTS)
	$(LD) $(LDFLAGS) -T $(LDSCRIPT) $(OBJECTS) -o $@

	@# Generate kernel symbols
	$(NM) -n $(TARGET) > $(BUILD_DIR)/symbols.map
	$(PYTHON) scripts/gen_symbols_asm.py $(BUILD_DIR)/symbols.map > $(BUILD_DIR)/symbols.S

	@# Compile the symbols to an object
	$(AS) $(ASFLAGS) -o $(BUILD_DIR)/symbols.o $(BUILD_DIR)/symbols.S

	@# Link again!
	$(LD) $(LDFLAGS) -T $(LDSCRIPT) $(OBJECTS) $(BUILD_DIR)/symbols.o -o $@

$(BUILD_DIR)/%.o: kernel/%.S
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: kernel/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.o: kernel/%.c kernel/%.h
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: qemu
qemu: $(TARGET)
	$(SHELL) scripts/qemu.sh $(TARGET)

.PHONY: objdump-kernel
objdump-kernel: $(TARGET)
	$(OBJDUMP) \
	--disassembler-color=extended \
	--visualize-jumps=off \
	-w -d -S \
	$(TARGET) | less -R