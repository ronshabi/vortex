.PHONY: all clean run
all:
	@$(MAKE) --no-print-directory -C kernel

clean:
	@$(MAKE) --no-print-directory -C kernel clean

run: all
	@bash scripts/qemu.sh