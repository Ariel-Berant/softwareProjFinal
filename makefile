CFLAGS = -ansi -Wall -Wextra -pedantic-errors -lm

symnmf: symnmf.o symnmf.h
	gcc -o symnmf.o vectorsFuncs.o matrixFuncs.o $(CFLAGS)

symnmf.o: symnmf.c
	gcc -c symnmf.c $(CFLAGS)

vectorsFuncs.o: vectorsFuncs.c
	gcc -c vectorsFuncs.c $(CFLAGS)

matrixFuncs.o: matrixFuncs.c
	gcc -c matrixFuncs.c $(CFLAGS)
