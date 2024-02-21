/*
 * This file contains the functionality to encode int64_t values into int63 and
 * decode them again.
 * The functionality is more or less identical to the commented out functionality
 * in file c/int63.c from
 * The main difference is that LL specifiers are used.
 */

// Include the prototypes of the functions defined here
#include <int64.h>

value encode_int63(int64_t x) {
    return (x << 1LL) | 1LL;
}

int64_t decode_int63(value x) {
    return (int64_t)(x >> 1LL);
}