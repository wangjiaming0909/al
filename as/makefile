all:
	as -o 1.o -gstabs 1.s
	ld -o a.out 1.o
	as86 -o 86.o 86.s
	ld86 -3 -N -o 2.out 86.o
