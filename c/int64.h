#ifndef CERTICOQ_INT64_H
#define CERTICOQ_INT64_H

// Include the value type
#include <values.h>
// Include standard integers since we use int64_t.
#include <certicoq_stdint.h>

// Encode an int64_t as int63
value encode_int63(int64_t x);

// Decode an int63 to int64_t
int64_t decode_int63(value x);

#endif //CERTICOQ_INT64_H