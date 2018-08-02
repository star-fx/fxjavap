#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "print_class.h"

/* check malloc result */
static void check_malloc_result(void *pointer) {
    if (pointer == NULL) {
        fprintf(stderr, "Error: malloc return NULL");
        exit(EXIT_FAILURE);
    }
}

static char *parse_utf8_info(struct cp_info *constant_pool, unsigned short index) {
    return constant_pool[index].info.utf8_info->bytes;
}

static char *parse_class_info(struct cp_info *constant_pool, unsigned short index) {
    return parse_utf8_info(constant_pool, constant_pool[index].info.class_info->name_index);
}

static char *parse_name_and_type_info(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_NameAndType_info *nameandtype = constant_pool[index].info.nameandtype_info;
    char *name = parse_utf8_info(constant_pool, nameandtype->name_index);
    char *descriptor = parse_utf8_info(constant_pool, nameandtype->descriptor_index);
    char *result = malloc(strlen(name) + strlen(descriptor) + 2);
    check_malloc_result(result);

    strcat(result, name);
    strcat(result, ":");
    strcat(result, descriptor);

    return result;
}

static char *parse_fieldref_info(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_Fieldref_info *fieldref = constant_pool[index].info.fieldref_info; 
    char *class = parse_class_info(constant_pool, fieldref->class_index);
    char *name_and_type = parse_name_and_type_info(constant_pool, fieldref->name_and_type_index);
    char *result = malloc(strlen(class) + strlen(name_and_type) + 2);
    check_malloc_result(result);

    strcpy(result, class);
    strcat(result, ".");
    strcat(result, name_and_type);
    
    return result;
}

static char *parse_methodref_info(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_Methodref_info *methodref = constant_pool[index].info.methodref_info; 
    char *class = parse_class_info(constant_pool, methodref->class_index);
    char *name_and_type = parse_name_and_type_info(constant_pool, methodref->name_and_type_index);
    char *result = malloc(strlen(class) + strlen(name_and_type) + 2);
    check_malloc_result(result);

    strcpy(result, class);
    strcat(result, ".");
    strcat(result, name_and_type);
    
    return result;
}

static char *parse_interface_methodref_info(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_Methodref_info *interface_methodref = constant_pool[index].info.methodref_info; 
    char *class = parse_class_info(constant_pool, interface_methodref->class_index);
    char *name_and_type = parse_name_and_type_info(constant_pool, interface_methodref->name_and_type_index);
    char *result = malloc(strlen(class) + strlen(name_and_type) + 2);
    check_malloc_result(result);

    strcpy(result, class);
    strcat(result, ".");
    strcat(result, name_and_type);
    
    return result;
}

static char *parse_string_info(struct cp_info *constant_pool, unsigned short index) {
    return parse_utf8_info(constant_pool, constant_pool[index].info.string_info->string_index);
}

static int parse_integer_info(struct cp_info *constant_pool, unsigned short index) {
    return constant_pool[index].info.integer_info->bytes;
}

static float parse_float_info(struct cp_info *constant_pool, unsigned short index) {
    return constant_pool[index].info.float_info->bytes;
}

static long parse_long_info(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_Long_info *long_info = constant_pool[index].info.long_info;
    return (long) long_info->high_bytes << 32 | long_info->low_bytes;
}

static double parse_double_info(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_Double_info *double_info = constant_pool[index].info.double_info;
    unsigned long l = (long) double_info->high_bytes << 32 | double_info->low_bytes;
    return *(double *)(&l);
}

static char *parse_methodtype_info(struct cp_info *constant_pool, unsigned short index) {
    return parse_utf8_info(constant_pool, constant_pool[index].info.methodtype_info->descriptor_index);
}

void print_class(Class *class) {
    int i;
    struct cp_info *constant_pool;

    printf("magic: %#X\n", class->magic);
    printf("minor_version: %hu\n", class->minor_version);
    printf("major_version: %hu\n", class->major_version);
    printf("Constant pool count: %hu\n", class->constant_pool_count);

    printf("Constant pool:\n");
    constant_pool = class->constant_pool;
    
    for (i = 1; i < class->constant_pool_count; i++) {
        switch (constant_pool[i].tag) {
            case CONSTANT_Class:
                printf("  #%-5d = %s // %s\n", i, "CONSTANT_Class", parse_class_info(constant_pool, i));
                break;
            case CONSTANT_Fieldref:
                printf("  #%-5d = %s // %s\n", i, "CONSTANT_Fieldref", parse_fieldref_info(constant_pool, i));
                break;
            case CONSTANT_Methodref:
                printf("  #%-5d = %s // %s\n", i, "CONSTANT_Methodref", parse_methodref_info(constant_pool, i));
                break;
            case CONSTANT_InterfaceMethodref:
                printf("  #%-5d = %s // %s\n", i, "CONSTANT_InterfaceMethodref", parse_interface_methodref_info(constant_pool, i));
                break;
            case CONSTANT_String:
                printf("  #%-5d = %s // %s\n", i, "CONSTANT_String", parse_string_info(constant_pool, i));
                break;
            case CONSTANT_Integer:
                printf("  #%-5d = %s // %d\n", i, "CONSTANT_Integer", parse_integer_info(constant_pool, i));
                break;
            case CONSTANT_Float:
                printf("  #%-5d = %s // %f\n", i, "CONSTANT_Float", parse_float_info(constant_pool, i));
                break;
            case CONSTANT_Long:
                printf("  #%-5d = %s // %ld\n", i, "CONSTANT_Long", parse_long_info(constant_pool, i));
                i++;
                break;
            case CONSTANT_Double:
                printf("  #%-5d = %s // %lf\n", i, "CONSTANT_Double", parse_double_info(constant_pool, i));
                i++;
                break;
            case CONSTANT_NameAndType:
                printf("  #%-5d = %s // %s\n", i, "CONSTANT_NameAndType", parse_name_and_type_info(constant_pool, i));
                break;
            case CONSTANT_Utf8:
                printf("  #%-5d = %s // %s\n", i, "CONSTANT_Utf8", parse_utf8_info(constant_pool, i));
                break;
            case CONSTANT_MethodHandle:
                printf("CONSTANT_MethodHandle\n");
                break;
            case CONSTANT_MethodType:
                printf("CONSTANT_MethodType");
                printf("  #%-5d = %s // %s\n", i, "CONSTANT_MethodType", parse_methodtype_info(constant_pool, i));
                break;
            case CONSTANT_InvokeDynamic:
                printf("CONSTANT_InvokeDynamic\n");
                break;
        }
    }

    printf("access_flags: %#X\n", class->access_flags);
    printf("this_class: %hu\n", class->this_class);
    printf("super_class: %hu\n", class->super_class);
    printf("interfaces_count: %hu\n", class->interfaces_count);

    for (i = 0; i < class->interfaces_count; i++) {
        printf("interface #%d value = %hu\n", i, *(class->interfaces++));
    }

    printf("fields_count: %hu\n", class->fields_count);
}