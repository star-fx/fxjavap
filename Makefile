fxjavap : class_file.o print_class.o
	gcc -o fxjavap src/fxjavap.c class_file.o print_class.o

class_file.o : src/class_file.c
	gcc -c src/class_file.c

print_class.o : src/print_class.c
	gcc -c src/print_class.c

clean :
	rm fxjavap class_file.o print_class.o
