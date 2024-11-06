CFLAGS = -ansi -Wall -Wextra -pedantic-errors -lm

symnmf: symnmf.o fileFuncs.o matrixFuncs.o symnmf.h
	gcc -g -o symnmf symnmf.o fileFuncs.o matrixFuncs.o $(CFLAGS)

symnmf.o: symnmf.c symnmf.h
	gcc -g -c symnmf.c $(CFLAGS)

fileFuncs.o: fileFuncs.c symnmf.h
	gcc -g -c fileFuncs.c $(CFLAGS)

matrixFuncs.o: matrixFuncs.c symnmf.h
	gcc -g -c matrixFuncs.c $(CFLAGS)

clean:
	rm -f symnmf symnmf.o fileFuncs.o matrixFuncs.o
