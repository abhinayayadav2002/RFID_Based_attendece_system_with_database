a.out: rfidmajor_main.o rfidmajor_fun.o
	cc rfidmajor_main.o rfidmajor_fun.o
rfidmajor_main.o: rfidmajor_main.c
	cc -c rfidmajor_main.c
rfidmajor_fun.o: rfidmajor_fun.c
	cc -c rfidmajor_fun.c

