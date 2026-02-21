#ifndef LIBLYR_H
#define LIBLYR_H

#define true 1
#define false 0

#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_end(ap)         __builtin_va_end(ap)
#define va_arg(ap, type)  __builtin_va_arg(ap, type)
#define va_copy(dest, src) __builtin_va_copy(dest, src)
typedef __builtin_va_list va_list;

typedef signed char        i8;
typedef unsigned char      u8;
typedef signed short       i16;
typedef unsigned short     u16;
typedef signed int         i32;
typedef unsigned int       u32;
typedef signed long long   i64;
typedef unsigned long long u64;

typedef unsigned long long st;

typedef u8 bool;

#define NULL ((void *) 0)
#endif