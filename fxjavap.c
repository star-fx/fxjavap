#include <stdio.h>
#include <stdlib.h>

#include "class_file.h"
#include "print_class.h"

void print_usage();

int main(int argc, char *argv[]) {
    FILE *class_file;
    Class *class;

    if (argc != 2) {
        print_usage();
        return EXIT_FAILURE;
    }
    
    class_file = fopen(argv[1], "rb");
    if (class_file == NULL) {
        fprintf(stderr, "Error: can not open file.\n");
        return EXIT_FAILURE;
    }

    class = parse_class(class_file);

    print_class(class);
}

void print_usage() {
    printf("Usage: fxjavap <class-file-path>\n");
}