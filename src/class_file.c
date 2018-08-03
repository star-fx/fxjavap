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
static u1 read_u1(FILE *class_file) {
    u1 _u1;
    fread(&_u1, sizeof(u1), 1, class_file);
    return _u1;
}

/* read u2*/
static u2 read_u2(FILE *class_file) {
    u2 _u2;
    u1 c2[2];

    fread(c2, sizeof(u1), 2, class_file);

    _u2 = (u2) c2[0] << 8;
    _u2 |= (u2) c2[1];

    return _u2;
}

/* read u4 */
static u4 read_u4(FILE *class_file) {
    u4 _u4;
    u1 c4[4];

    fread(c4, sizeof(u1), 4, class_file);

    _u4 = (u4) c4[0] << 24;
    _u4 |= (u4) c4[1] << 16;
    _u4 |= (u4) c4[2] << 8;
    _u4 |= (u4) c4[3];

    return _u4;
}

/* read string */
static char *read_string(FILE *class_file, u2 length) {
    char *string = malloc(sizeof(char) * (length + 1));
    check_malloc_result(string);
    fread(string, sizeof(char), length, class_file);
    *(string + length) = '\0';

    return string;
}

/* read constant pool info */
static struct cp_info *read_constant_pool(FILE *class_file, u2 constant_pool_count) {
    struct cp_info *constant_pool;
    int i;
    u1 tag;

    constant_pool = malloc(sizeof(struct cp_info) * constant_pool_count);
    check_malloc_result(constant_pool);

    for (i = 1; i < constant_pool_count; i++) {
        tag = read_u1(class_file);
        constant_pool[i].tag = tag;

        switch (tag) { 
            case CONSTANT_Class:
                constant_pool[i].info.class_info = malloc(sizeof(struct CONSTANT_Class_info));
                check_malloc_result(constant_pool[i].info.class_info);
                constant_pool[i].info.class_info->name_index = read_u2(class_file);
                break;
            case CONSTANT_Fieldref:
                constant_pool[i].info.fieldref_info = malloc(sizeof(struct CONSTANT_Fieldref_info));
                constant_pool[i].info.fieldref_info->class_index = read_u2(class_file);
                constant_pool[i].info.fieldref_info->name_and_type_index = read_u2(class_file);
                break;
            case CONSTANT_Methodref:
                constant_pool[i].info.methodref_info = malloc(sizeof(struct CONSTANT_Methodref_info));
                constant_pool[i].info.methodref_info->class_index = read_u2(class_file);
                constant_pool[i].info.methodref_info->name_and_type_index = read_u2(class_file);
                break;
            case CONSTANT_InterfaceMethodref:
                /* TODO: make test */
                constant_pool[i].info.interfacemethodref_info = malloc(sizeof(struct CONSTANT_InterfaceMethodref_info));
                constant_pool[i].info.interfacemethodref_info->class_index = read_u2(class_file);
                constant_pool[i].info.interfacemethodref_info->name_and_type_index = read_u2(class_file);
                break;
            case CONSTANT_String:
                constant_pool[i].info.string_info = malloc(sizeof(struct CONSTANT_String_info));
                constant_pool[i].info.string_info->string_index = read_u2(class_file);
                break;
            case CONSTANT_Integer:
                constant_pool[i].info.integer_info = malloc(sizeof(struct CONSTANT_Integer_info));
                constant_pool[i].info.integer_info->bytes = read_u4(class_file);
                break;
            case CONSTANT_Float:
                constant_pool[i].info.float_info = malloc(sizeof(struct CONSTANT_Float_info));
                constant_pool[i].info.float_info->bytes = read_u4(class_file);
                break;
            case CONSTANT_Long:
                constant_pool[i].info.long_info = malloc(sizeof(struct CONSTANT_Long_info));
                constant_pool[i].info.long_info->high_bytes = read_u4(class_file);
                constant_pool[i].info.long_info->low_bytes = read_u4(class_file);
                i++; /* long type require two space */
                break;
            case CONSTANT_Double:
                constant_pool[i].info.double_info = malloc(sizeof(struct CONSTANT_Double_info));
                constant_pool[i].info.double_info->high_bytes = read_u4(class_file);
                constant_pool[i].info.double_info->low_bytes = read_u4(class_file);
                i++; /* double type require two space */
                break;
            case CONSTANT_NameAndType:
                constant_pool[i].info.nameandtype_info = malloc(sizeof(struct CONSTANT_NameAndType_info));
                constant_pool[i].info.nameandtype_info->name_index = read_u2(class_file);
                constant_pool[i].info.nameandtype_info->descriptor_index = read_u2(class_file);
                break;
            case CONSTANT_Utf8:
                constant_pool[i].info.utf8_info = malloc(sizeof(struct CONSTANT_Utf8_info));
                constant_pool[i].info.utf8_info->length = read_u2(class_file);
                constant_pool[i].info.utf8_info->bytes = read_string(class_file, constant_pool[i].info.utf8_info->length);
                break;
            case CONSTANT_MethodHandle:
                /* TODO: make test */
                constant_pool[i].info.methodhandle_info = malloc(sizeof(struct CONSTANT_Methodhandle_info));
                constant_pool[i].info.methodhandle_info->reference_kind = read_u1(class_file);
                constant_pool[i].info.methodhandle_info->reference_index = read_u2(class_file);
                break;
            case CONSTANT_MethodType:
                /* TODO: make test */
                constant_pool[i].info.methodtype_info = malloc(sizeof(struct CONSTANT_MethodType_info));
                constant_pool[i].info.methodtype_info->descriptor_index = read_u2(class_file);
                break;
            case CONSTANT_InvokeDynamic:
                /* TODO: make test */
                constant_pool[i].info.invokedynamic_info = malloc(sizeof(struct CONSTANT_InvokeDynamic_info));
                constant_pool[i].info.invokedynamic_info->bootstrap_method_attr_index = read_u2(class_file);
                constant_pool[i].info.invokedynamic_info->name_and_type_index = read_u2(class_file);
                break;
        }
    }
    
    return constant_pool;
}

static u2 *read_interfaces(FILE *class_file, u2 interfaces_count) {
    int i;
    u2 *interfaces = malloc(sizeof(u2) * interfaces_count);
    check_malloc_result(interfaces);

    for (i = 0; i < interfaces_count; i++) {
        interfaces[i] = read_u2(class_file);
    }

    return interfaces;
}

Class *read_class(FILE *class_file) {
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
    class->interfaces_count = read_u2(class_file);
    class->interfaces = read_interfaces(class_file, class->interfaces_count);
    class->fields_count = read_u2(class_file);

    return class;
}