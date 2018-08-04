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

static int get_number_length(unsigned short number) {
    int length = 1;
    while (number / 10 != 0) {
        length++;
        number /= 10;
    }
    return length;
}

static void print_spaces(int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf(" ");
    }
}

static char *parse_utf8_info(struct cp_info *constant_pool, unsigned short index) {
    return constant_pool[index].info.utf8_info->bytes;
}

static char *parse_class_info(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_Class_info *class = constant_pool[index].info.class_info;
    char *result = malloc(get_number_length(sizeof(char) * class->name_index) + 2); 
    check_malloc_result(result);
    sprintf(result, "#%d", class->name_index);
    return result;
}

static char *parse_class_info_verbose(struct cp_info *constant_pool, unsigned short index) {
    return parse_utf8_info(constant_pool, constant_pool[index].info.class_info->name_index);
}

static char *parse_name_and_type_info(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_NameAndType_info *nameandtype = constant_pool[index].info.nameandtype_info;
    char * result = malloc(sizeof(char) * (get_number_length(nameandtype->name_index) + get_number_length(nameandtype->descriptor_index) + 4));
    check_malloc_result(result);
    sprintf(result, "#%d:#%d", nameandtype->name_index, nameandtype->descriptor_index);
    return result;
}

static char *parse_name_and_type_info_verbose(struct cp_info *constant_pool, unsigned short index) {
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
    char * result = malloc(sizeof(char) * (get_number_length(fieldref->class_index) + get_number_length(fieldref->name_and_type_index) + 4));
    check_malloc_result(result);
    sprintf(result, "#%d.#%d", fieldref->class_index, fieldref->name_and_type_index);
    return result;
}

static char *parse_fieldref_info_verbose(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_Fieldref_info *fieldref = constant_pool[index].info.fieldref_info; 
    char *class = parse_class_info_verbose(constant_pool, fieldref->class_index);
    char *name_and_type = parse_name_and_type_info_verbose(constant_pool, fieldref->name_and_type_index);
    char *result = malloc(strlen(class) + strlen(name_and_type) + 2);
    check_malloc_result(result);

    strcpy(result, class);
    strcat(result, ".");
    strcat(result, name_and_type);
    
    return result;
}

static char *parse_methodref_info(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_Methodref_info *methodref = constant_pool[index].info.methodref_info; 
    char *result = malloc(sizeof(char) * (get_number_length(methodref->class_index) + get_number_length(methodref->name_and_type_index) + 4));
    check_malloc_result(result);
    sprintf(result, "#%d.#%d", methodref->class_index, methodref->name_and_type_index);
    return result;
}

static char *parse_methodref_info_verbose(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_Methodref_info *methodref = constant_pool[index].info.methodref_info; 
    char *class = parse_class_info_verbose(constant_pool, methodref->class_index);
    char *name_and_type = parse_name_and_type_info_verbose(constant_pool, methodref->name_and_type_index);
    char *result = malloc(strlen(class) + strlen(name_and_type) + 2);
    check_malloc_result(result);

    strcpy(result, class);
    strcat(result, ".");
    strcat(result, name_and_type);
    
    return result;
}

static char *parse_interface_methodref_info(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_Methodref_info *interface_methodref = constant_pool[index].info.methodref_info; 
    char *result = malloc(sizeof(char) * ((get_number_length(interface_methodref->class_index)) + get_number_length(interface_methodref->name_and_type_index) + 4));
    check_malloc_result(result);
    sprintf(result, "#%d.#%d", interface_methodref->class_index, interface_methodref->name_and_type_index);
    return result;
}

static char *parse_interface_methodref_info_verbose(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_Methodref_info *interface_methodref = constant_pool[index].info.methodref_info; 
    char *class = parse_class_info_verbose(constant_pool, interface_methodref->class_index);
    char *name_and_type = parse_name_and_type_info_verbose(constant_pool, interface_methodref->name_and_type_index);
    char *result = malloc(strlen(class) + strlen(name_and_type) + 2);
    check_malloc_result(result);

    strcpy(result, class);
    strcat(result, ".");
    strcat(result, name_and_type);
    
    return result;
}

static char *parse_string_info(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_String_info *string = constant_pool[index].info.string_info;
    char * result = malloc(sizeof(char) * (get_number_length(string->string_index) + 2));
    check_malloc_result(result);
    sprintf(result, "#%d", string->string_index);
    return result;
}

static char *parse_string_info_verbose(struct cp_info *constant_pool, unsigned short index) {
    return parse_utf8_info(constant_pool, constant_pool[index].info.string_info->string_index);
}

static int parse_integer_info(struct cp_info *constant_pool, unsigned short index) {
    return *(int *)(&constant_pool[index].info.integer_info->bytes);
}

static float parse_float_info(struct cp_info *constant_pool, unsigned short index) {
    return *(float *)(&constant_pool[index].info.float_info->bytes);
}

static long parse_long_info(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_Long_info *long_info = constant_pool[index].info.long_info;
    return (long) long_info->high_bytes << 32 | long_info->low_bytes;
}

static double parse_double_info(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_Double_info *double_info = constant_pool[index].info.double_info;
    unsigned long l = (unsigned long) double_info->high_bytes << 32 | double_info->low_bytes;
    return *(double *)(&l);
}

static char *parse_methodtype_info(struct cp_info *constant_pool, unsigned short index) {
    struct CONSTANT_MethodType_info *methodtype = constant_pool[index].info.methodtype_info;
    char *result = malloc(sizeof(char) * (get_number_length(methodtype->descriptor_index) + 2));
    check_malloc_result(result);
    sprintf(result, "#%d", methodtype->descriptor_index);
    return result;
}

static char *parse_methodtype_info_verbose(struct cp_info *constant_pool, unsigned short index) {
    return parse_utf8_info(constant_pool, constant_pool[index].info.methodtype_info->descriptor_index);
}

static void print_access_flags(Class *class) {
    if (class->access_flags & ACC_PUBLIC) {
        printf("public ");
    }
    if (class->access_flags & ACC_FINAL) {
        printf("final ");
    }
    if(class->access_flags & ACC_INTERFACE) {
        printf("interface ");
    } else if (class->access_flags & ACC_ANNOTATION) {
        printf("@interface ");
    } else if (class->access_flags & ACC_ENUM) {
        printf("enum ");
    } else {
        if (class->access_flags & ACC_ABSTRACT) {
            printf("abstract ");
        }
        printf("class ");
    }
}

void print_class(Class *class) {
    char *super_class;
    int i;
    struct cp_info *constant_pool = class->constant_pool;
    struct field_info *fields = class->fields;
    struct method_info *methods = class->methods;
    struct attribute_info *attributes = class->attributes;

    print_access_flags(class);
    printf("%s ", parse_class_info(constant_pool, class->this_class));
    super_class = parse_class_info(constant_pool, class->super_class);
    if (strcmp(super_class, "java/lang/Object") != 0) {
        printf("extends %s ", super_class);
    }
    if (class->interfaces_count > 0) {
        printf("implements ");
        for (i = 0; i < class->interfaces_count; i++) {
            printf("%s", parse_class_info(constant_pool, class->interfaces[i]));
            if (i != class->interfaces_count - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }

    printf("  minor_version: %hu\n", class->minor_version);

    printf("  major_version: %hu\n", class->major_version);

    printf("Constant pool:\n");
    for (i = 1; i < class->constant_pool_count; i++) {
        print_spaces(get_number_length(class->constant_pool_count - 1) - get_number_length(i) + 2);
        switch (constant_pool[i].tag) {
            case CONSTANT_Class:
                printf("#%d = %-18s %-13s // %s\n", i, "Class", parse_class_info(constant_pool, i), parse_class_info_verbose(constant_pool, i));
                break;
            case CONSTANT_Fieldref:
                printf("#%d = %-18s %-13s // %s\n", i, "Fieldref", parse_fieldref_info(constant_pool, i), parse_fieldref_info_verbose(constant_pool, i));
                break;
            case CONSTANT_Methodref:
                printf("#%d = %-18s %-13s // %s\n", i, "Methodref", parse_methodref_info(constant_pool, i), parse_methodref_info_verbose(constant_pool, i));
                break;
            case CONSTANT_InterfaceMethodref:
                printf("#%d = %-18s // %s\n", i, "InterfaceMethodref", parse_interface_methodref_info(constant_pool, i));
                break;
            case CONSTANT_String:
                printf("#%d = %-18s %-13s // %s\n", i, "String", parse_string_info(constant_pool, i), parse_string_info_verbose(constant_pool, i));
                break;
            case CONSTANT_Integer:
                printf("#%d = %-18s %d\n", i, "Integer", parse_integer_info(constant_pool, i));
                break;
            case CONSTANT_Float:
                printf("#%d = %-18s %f\n", i, "Float", parse_float_info(constant_pool, i));
                break;
            case CONSTANT_Long:
                printf("#%d = %-18s %ld\n", i, "Long", parse_long_info(constant_pool, i));
                i++;
                break;
            case CONSTANT_Double:
                printf("#%d = %-18s %lf\n", i, "Double", parse_double_info(constant_pool, i));
                i++;
                break;
            case CONSTANT_NameAndType:
                printf("#%d = %-18s %-13s // %s\n", i, "NameAndType", parse_name_and_type_info(constant_pool, i), parse_name_and_type_info_verbose(constant_pool, i));
                break;
            case CONSTANT_Utf8:
                printf("#%d = %-18s %s\n", i, "Utf8", parse_utf8_info(constant_pool, i));
                break;
            case CONSTANT_MethodHandle:
                printf("#%d = %-18s\n", i, "MethodHandle");
                break;
            case CONSTANT_MethodType:
                printf("#%d = %-18s %-13s // %s\n", i, "MethodType", parse_methodtype_info(constant_pool, i), parse_methodtype_info_verbose(constant_pool, i));
                break;
            case CONSTANT_InvokeDynamic:
                printf("#%d = %-18s\n", i, "InvokeDynamic");
                break;
        }
    }

    printf("{\n");

    for (i = 0; i < class->methods_count; i++) {
        printf("method #%d %s %s\n", i, parse_utf8_info(constant_pool, methods[i].name_index), parse_utf8_info(constant_pool, methods[i].descriptor_index));
    }

    printf("}\n");
}