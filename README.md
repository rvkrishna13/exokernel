To run this code we need to install qemu and riscv gnu toolchain.

Commands to start the operating system:

cd xv6-riscv
make clean
make
make qemu





To test the functionality:

Testing STLB:

We provide the application with the options to use STLB or not. It can be done by using the use_stlb command line arguments

We have a test suite to test the performance of STLB(user/usertests.c)
To use STLB:
	run
	usertests use_stlb=1
Without STLB:
	run
	usertests use_stlb=0