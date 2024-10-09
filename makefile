CFLAGS = -ansi -Wall -Wextra -pedantic-errors -lm

symnmf: symnmf.o vectorFuncs.o matrixFuncs.o symnmf.h
	gcc -g -o symnmf symnmf.o vectorFuncs.o matrixFuncs.o $(CFLAGS)

symnmf.o: symnmf.c symnmf.h
	gcc -g -c symnmf.c $(CFLAGS)

vectorFuncs.o: vectorFuncs.c symnmf.h
	gcc -g -c vectorFuncs.c $(CFLAGS)

matrixFuncs.o: matrixFuncs.c symnmf.h
	gcc -g -c matrixFuncs.c $(CFLAGS)
