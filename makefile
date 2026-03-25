assembler: main.o kdamassembler.o functions.o check_assembler_first.o instFunction.o check_assembler_second.o funcSecPass.o
	gcc -ansi -Wall -pedantic main.o kdamassembler.o functions.o check_assembler_first.o instFunction.o check_assembler_second.o funcSecPass.o -o assembler 

main.o: main.c data.h
	gcc -c -ansi -Wall -pedantic main.c

kdamassembler.o: kdamassembler.c data.h
	gcc -c -ansi -Wall -pedantic kdamassembler.c

functions.o: functions.c data.h
	gcc -c -ansi -Wall -pedantic functions.c

check_assembler_first.o: check_assembler_first.c data.h
	gcc -c -ansi -Wall -pedantic check_assembler_first.c

instFunction.o: instFunction.c data.h
	gcc -c -ansi -Wall -pedantic instFunction.c

funcSecPass.o: funcSecPass.c data.h
	gcc -c -ansi -Wall -pedantic funcSecPass.c

check_assembler_second.o: check_assembler_second.c data.h
	gcc -c -ansi -Wall -pedantic check_assembler_second.c



