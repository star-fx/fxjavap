#include <stdio.h>

int main(int argc, char *argv[]) {
    unsigned int magic;

    class_file_path = "/Users/star_fx/Downloads/HelloWorld.java";
    FILE class_file = fopen(class_file_path, "rb");

    fscanf(class_file, "%1d", &magic);
    printf("magic: %#x\n", magic);

}
