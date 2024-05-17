#ifndef CERTICOQ_INT6X_N_H
#define CERTICOQ_INT6X_N_H

// Include the value type
#include <values.h>
// Include standard integers since we use int64_t.
#include <certicoq_stdint.h>

// Forward declare the thread info.
extern struct thread_info *tinfo;

value uint63_from_N(value n);

uint64_t uint64_from_N(value n);

value uint63_to_N(struct thread_info *tinfo, value t);

value uint64_to_N(struct thread_info *tinfo, uint64_t t);

#endif // CERTICOQ_INT6X_N_H