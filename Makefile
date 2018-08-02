CC = gcc
VPATH = src

objects = fxjavap.o class_file.o print_class.o

fxjavap : $(objects)
	$(CC) $(objects) -o fxjavap

$(objects): %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY : clean
clean :
	-rm fxjavap $(objects)

