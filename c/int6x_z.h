#ifndef CERTICOQ_INT6X_Z_H
#define CERTICOQ_INT6X_Z_H

// Include the value type
#include <values.h>
// Include standard integers since we use int64_t.
#include <certicoq_stdint.h>

// Forward declare the thread info.
extern struct thread_info *tinfo;

value uint63_from_Z(value z);

int64_t int64_from_Z(value z);

value uint63_to_Z(struct thread_info *tinfo, value t);

value int64_to_Z(struct thread_info *tinfo, int64_t t);

#endif // CERTICOQ_INT6X_Z_H