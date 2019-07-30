#pragma once

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

typedef float r32;
typedef double r64;

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define ARRAY_COPY(source, destination, size) {\
    /*  assert(ARRAY_SIZE(destination) >= size);  can't use this assert with simple pointers :( */ \
    assert(ARRAY_SIZE(source) <= size); \
    for(int i = 0; i < size; ++i){\
        destination[i] = source[i];\
    } \
}
