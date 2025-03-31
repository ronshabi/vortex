.PHONY: all
all:
	@$(MAKE) --no-print-directory -C kernel

.PHONY: clean
clean:
	@$(MAKE) --no-print-directory -C kernel clean

.PHONY: qemu
qemu: all
	@bash scripts/qemu.sh

.PHONY: format
format:
	@bash scripts/format.sh


