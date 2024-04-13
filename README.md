# exokernel on mac

# installing riscv-gnu-toolchain

$ brew tap riscv-software-src/riscv
$ brew install riscv-tools

# installing qemu

brew install qemu





# Debugging Commands

gdb-multiarch
set architecture riscv:rv64
set disassemble-next-line on
target remote :1234