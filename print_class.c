#include <stdio.h>

#include "print_class.h"

void print_class(Class *class) {
    int i;
    struct cp_info *constant_pool;

    printf("magic: %#X\n", class->magic);
    printf("minor_version: %hu\n", class->minor_version);
    printf("major_version: %hu\n", class->major_version);
    printf("constant_pool_count: %hu\n", class->constant_pool_count);

    constant_pool = class->constant_pool;
    
    for (i = 1; i < class->constant_pool_count; i++) {
        printf("  #%d tag = %d type = ", i, constant_pool[i].tag);
        switch (constant_pool[i].tag) {
            case CONSTANT_Class: printf("CONSTANT_Class\n"); break;
            case CONSTANT_Fieldref: printf("CONSTANT_Fieldref\n"); break;
            case CONSTANT_Methodref: printf("CONSTANT_Methodref\n"); break;
            case CONSTANT_InterfaceMethodref: printf("CONSTANT_InterfaceMethodref\n"); break;
            case CONSTANT_String: printf("CONSTANT_String\n"); break;
            case CONSTANT_Integer: printf("CONSTANT_Integer\n"); break;
            case CONSTANT_Float: printf("CONSTANT_Float\n"); break;
            case CONSTANT_Long: printf("CONSTANT_Long\n"); break;
            case CONSTANT_Double: printf("CONSTANT_Double\n"); break;
            case CONSTANT_NameAndType: printf("CONSTANT_NameAndType\n"); break;
            case CONSTANT_Utf8: printf("CONSTANT_Utf8\n"); break;
            case CONSTANT_MethodHandle: printf("CONSTANT_MethodHandle\n"); break;
            case CONSTANT_MethodType: printf("CONSTANT_MethodType"); break;
            case CONSTANT_InvokeDynamic: printf("CONSTANT_InvokeDynamic\n"); break;
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