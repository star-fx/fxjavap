#include <stdio.h>
#include <stdlib.h>

#include "class_file.h"

/* check malloc result */
static void check_malloc_result(void *pointer) {
    if (pointer == NULL) {
        fprintf(stderr, "Error: malloc return NULL");
        exit(EXIT_FAILURE);
    }
}

/* read u1 */
static unsigned char read_u1(FILE *class_file) {
    unsigned char u1;
    fread(&u1, sizeof(char), 1, class_file); 
    return u1;
}

/* read u2*/
static unsigned short read_u2(FILE *class_file) {
    unsigned short u2;
    unsigned char c2[2];

    fread(c2, sizeof(char), 2, class_file);

    u2 = (unsigned short) c2[0] << 8;
    u2 |= (unsigned short) c2[1];

    return u2;
}

/* read u4 */
static unsigned int read_u4(FILE *class_file) {
    unsigned int u4;
    unsigned char c4[4];

    fread(c4, sizeof(char), 4, class_file);

    u4 = (unsigned int) c4[0] << 24;
    u4 |= (unsigned int) c4[1] << 16;
    u4 |= (unsigned int) c4[2] << 8;
    u4 |= (unsigned int) c4[3];

    return u4;
}

/* read string */
static unsigned char *read_string(FILE *class_file, unsigned short length) {
    unsigned char *string = malloc(sizeof(char) * (length + 1));
    check_malloc_result(string);
    fread(string, sizeof(char), length, class_file);

    return string;
}

/* read constant pool info */
static struct cp_info *read_constant_pool(FILE *class_file, unsigned short constant_pool_count) {
    struct cp_info *head, *first, *current;
    unsigned char tag; 
    int i;

    first = malloc(sizeof(struct cp_info));
    check_malloc_result(first);
    head = first;
    
    for (i = 1; i < constant_pool_count; i++) {
        current = malloc(sizeof(struct cp_info));
        check_malloc_result(current);

        tag = read_u1(class_file);
        current->tag = tag;

        switch (tag) { 
            case CONSTANT_Class:
                current->info.constant_class_info = malloc(sizeof(struct CONSTANT_Class_info));
                current->info.constant_class_info->name_index = read_u2(class_file);
                break;
            case CONSTANT_Fieldref:
                current->info.constant_fieldref_info = malloc(sizeof(struct CONSTANT_Fieldref_info)); 
                current->info.constant_fieldref_info->class_index = read_u2(class_file);
                current->info.constant_fieldref_info->name_and_type_index = read_u2(class_file);
                break;
            case CONSTANT_Methodref:
                current->info.constant_methodref_info = malloc(sizeof(struct CONSTANT_Methodref_info));
                current->info.constant_methodref_info->class_index = read_u2(class_file);
                current->info.constant_methodref_info->name_and_type_index = read_u2(class_file);
                break;
            case CONSTANT_InterfaceMethodref:
                /* TODO: make test */
                current->info.constant_interfacemethodref_info = malloc(sizeof(struct CONSTANT_InterfaceMethodref_info));
                current->info.constant_interfacemethodref_info->class_index = read_u2(class_file);
                current->info.constant_interfacemethodref_info->name_and_type_index = read_u2(class_file);
                break;
            case CONSTANT_String:
                current->info.constant_string_info = malloc(sizeof(struct CONSTANT_String_info));
                current->info.constant_string_info->string_index = read_u2(class_file);
                break;
            case CONSTANT_Integer:
                current->info.constant_integer_info = malloc(sizeof(struct CONSTANT_Integer_info));
                current->info.constant_integer_info->bytes = read_u4(class_file);
                break;
            case CONSTANT_Float:
                /* TODO: make test */
                current->info.constant_float_info = malloc(sizeof(struct CONSTANT_Float_info));
                current->info.constant_float_info->bytes = read_u4(class_file);
                break;
            case CONSTANT_Long:
                /* TODO: make test */
                current->info.constant_long_info = malloc(sizeof(struct CONSTANT_Long_info));
                current->info.constant_long_info->high_bytes = read_u4(class_file);
                current->info.constant_long_info->low_bytes = read_u4(class_file);
                break;
            case CONSTANT_Double:
                /* TODO: make test */
                current->info.constant_double_info = malloc(sizeof(struct CONSTANT_Double_info)); 
                current->info.constant_double_info->high_bytes = read_u4(class_file);
                current->info.constant_double_info->low_bytes = read_u4(class_file);
                break;
            case CONSTANT_NameAndType:
                current->info.constant_nameandtype_info = malloc(sizeof(struct CONSTANT_NameAndType_info));
                current->info.constant_nameandtype_info->name_index = read_u2(class_file);
                current->info.constant_nameandtype_info->descriptor_index = read_u2(class_file);
                break;
            case CONSTANT_Utf8:
                current->info.constant_utf8_info = malloc(sizeof(struct CONSTANT_Utf8_info));
                current->info.constant_utf8_info->length = read_u2(class_file);
                current->info.constant_utf8_info->bytes = read_string(class_file, current->info.constant_utf8_info->length);
                break;
            case CONSTANT_MethodHandle:
                /* TODO: make test */
                current->info.constant_methodhandle_info = malloc(sizeof(struct CONSTANT_Methodhandle_info));
                current->info.constant_methodhandle_info->reference_kind = read_u1(class_file);
                current->info.constant_methodhandle_info->reference_index = read_u2(class_file);
                break;
            case CONSTANT_MethodType:
                /* TODO: make test */
                current->info.constant_methodtype_info = malloc(sizeof(struct CONSTANT_MethodType_info));
                current->info.constant_methodtype_info->descriptor_index = read_u2(class_file);
                break;
            case CONSTANT_InvokeDynamic:
                /* TODO: make test */
                current->info.constant_invokedynamic_info = malloc(sizeof(struct CONSTANT_InvokeDynamic_info));
                current->info.constant_invokedynamic_info->bootstrap_method_attr_index = read_u2(class_file);
                current->info.constant_invokedynamic_info->name_and_type_index = read_u2(class_file);
                break;
        }

        first->next = current;
        first = current;
    }
    
    return head;
}

Class *parse_class(FILE *class_file) {
    Class *class = malloc(sizeof(Class));
    check_malloc_result(class);

    class->magic = read_u4(class_file);

    class->minor_version = read_u2(class_file);

    class->major_version = read_u2(class_file);

    class->constant_pool_count = read_u2(class_file);

    class->constant_pool = read_constant_pool(class_file, class->constant_pool_count);

    class->access_flags = read_u2(class_file);

    class->this_class = read_u2(class_file);

    class->super_class = read_u2(class_file);

    return class;
}