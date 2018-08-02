fxjavap : class_file.o print_class.o
	gcc -o fxjavap fxjavap.c class_file.o print_class.o

class_file.o : class_file.c
	gcc -c class_file.c

print_class.o : print_class.c
	gcc -c print_class.c

clean :
	rm fxjavap class_file.o print_class.o
