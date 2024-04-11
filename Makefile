debug:
	qemu-system-riscv64 -s -S \
	-machine virt \
	-nographic \
	-bios opensbi/build/platform/generic/firmware/fw_dynamic.bin

start:
	qemu-system-riscv64 \
	-machine virt \
	-nographic \
	-bios opensbi/build/platform/generic/firmware/fw_dynamic.bin