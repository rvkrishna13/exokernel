To run this code we need to install qemu and riscv gnu toolchain.

Commands to start the operating system:

cd xv6-riscv <br>
make clean   <br>
make       <br>
make qemu  <br>





To test the functionality:

Testing STLB:

We provide the application with the options to use STLB or not. It can be done by using the use_stlb command line arguments

We have a test suite to test the performance of STLB(user/usertests.c) <br>
To use STLB:<br>
	run<br>
	usertests use_stlb=1<br>
Without STLB:<br>
	run<br>
	usertests use_stlb=0
