#ifndef __DATATYPE_H__
#define __DATATYPE_H__

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char bool;

#ifndef __cplusplus
    #define true 1
    #define false 0
#endif

#define ITEMS_OF(array) (sizeof(array)/sizeof(array[0]))

#endif // __DATATYPE_H__
