#ifndef CLASS_H
#define CLASS_H

#include <stdio.h>

/* basic typedef */
typedef unsigned char u1;
typedef unsigned short u2;
typedef unsigned int u4;

/* constant pool tag type */
#define CONSTANT_Class                 7
#define CONSTANT_Fieldref              9
#define CONSTANT_Methodref             10
#define CONSTANT_InterfaceMethodref    11
#define CONSTANT_String                8
#define CONSTANT_Integer               3
#define CONSTANT_Float                 4
#define CONSTANT_Long                  5
#define CONSTANT_Double                6
#define CONSTANT_NameAndType           12
#define CONSTANT_Utf8                  1
#define CONSTANT_MethodHandle          15
#define CONSTANT_MethodType            16
#define CONSTANT_InvokeDynamic         18

/* constant pool type */
struct CONSTANT_Class_info { /* tag: 7 */
    u2 name_index;
}; 

struct CONSTANT_Fieldref_info { /* tag: 9 */
    u2 class_index;
    u2 name_and_type_index;
};

struct CONSTANT_Methodref_info { /* tag: 10 */
    u2 class_index;
    u2 name_and_type_index;
};

struct CONSTANT_InterfaceMethodref_info { /* tag: 11 */
    u2 class_index;
    u2 name_and_type_index;
};

struct CONSTANT_String_info { /* tag: 8 */
    u2 string_index;
};

struct CONSTANT_Integer_info { /* tag: 3 */
    u4 bytes;
};

struct CONSTANT_Float_info { /* tag: 4 */
    u4 bytes;
};

struct CONSTANT_Long_info { /* tag: 5 */
    u4 high_bytes;
    u4 low_bytes;
};

struct CONSTANT_Double_info { /* tag: 6 */
    u2 high_bytes;
    u2 low_bytes;
};

struct CONSTANT_NameAndType_info { /* tag: 12 */
    u2 name_index;
    u2 descriptor_index;
};

struct CONSTANT_Utf8_info { /* tag: 1 */
    u2 length;
    char *bytes;
};

struct CONSTANT_Methodhandle_info { /* tag: 15 */
    u1 reference_kind;
    u2 reference_index;
};

struct CONSTANT_MethodType_info { /* tag: 16 */
    u2 descriptor_index;
};

struct CONSTANT_InvokeDynamic_info { /* tag: 18 */
    u2 bootstrap_method_attr_index;
    u2 name_and_type_index;
};

/* constant pool */
struct cp_info {
    u1 tag;
    union {
        struct CONSTANT_Class_info *class_info; /* tag: 7 */
        struct CONSTANT_Fieldref_info *fieldref_info; /* tag: 9 */
        struct CONSTANT_Methodref_info *methodref_info; /* tag: 10 */
        struct CONSTANT_InterfaceMethodref_info *interfacemethodref_info; /* tag: 11 */
        struct CONSTANT_String_info *string_info; /* tag: 8 */
        struct CONSTANT_Integer_info *integer_info; /* tag: 3 */
        struct CONSTANT_Float_info *float_info; /* tag: 4 */
        struct CONSTANT_Long_info *long_info; /* tag: 5 */
        struct CONSTANT_Double_info *double_info; /* tag:6 */
        struct CONSTANT_NameAndType_info *nameandtype_info; /* tag: 12 */
        struct CONSTANT_Utf8_info *utf8_info; /* tag: 1 */
        struct CONSTANT_Methodhandle_info *methodhandle_info; /* tag: 15 */
        struct CONSTANT_MethodType_info *methodtype_info; /* tag: 16 */
        struct CONSTANT_InvokeDynamic_info *invokedynamic_info; /* tag: 18 */
    } info;
};

/* class file */
typedef struct Class {
    u4 magic;
    u2 minor_version;
    u2 major_version;
    u2 constant_pool_count;
    struct cp_info *constant_pool;
    u2 access_flags;
    u2 this_class;
    u2 super_class;
    u2 interfaces_count;
    u2 *interfaces;
    u2 fields_count;
} Class;

/* parse class file */
Class *read_class(FILE *class_file);

#endif
