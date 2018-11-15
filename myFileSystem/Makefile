myFileSystem: Tree.o Map.o dataTypes.o Func.o main.o
	gcc Tree.o Func.o Map.o dataTypes.o main.o -o myFileSystem

Tree.o: Tree.c Tree.h 
	gcc -c Tree.c

Func.o: Func.c Func.h 
	gcc -c Func.c

Map.o:  Map.c Map.h
	gcc -c Map.c

dataTypes.o: dataTypes.c dataTypes.h 
	gcc -c dataTypes.c

main.o: main.c
	gcc -c main.c

remove:
	rm -f *.o


