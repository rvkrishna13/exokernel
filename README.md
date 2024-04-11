# exokernel on mac

# installing riscv-gnu-toolchain

$ brew tap riscv-software-src/riscv
$ brew install riscv-tools

# installing qemu

https://theintobooks.wordpress.com/2019/07/09/installing-risc-v-qemu-on-macos-mojave/ 





# Debugging Commands

gdb-multiarch
set architecture riscv:rv64
set disassemble-next-line on
target remote :1234