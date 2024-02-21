#ifndef CERTICOQ_INT6X_NAT_H
#define CERTICOQ_INT6X_NAT_H

// Include the value type
#include <values.h>
// Include standard integers since we use int64_t.
#include <certicoq_stdint.h>

// Forward declare the thread info.
extern struct thread_info *tinfo;

// Transforming a nat into uint63
value uint63_from_nat(value n);

// Transforming a nat into int64_t
int64_t int64_from_nat(value n);

// Transforming an uint63 to nat
value uint63_to_nat(struct thread_info *tinfo, value t);

// Transforming an int64_t to nat
// CAUTION: using this function is only safe if t guaranteed to be non-negative
value int64_to_nat(struct thread_info *tinfo, int64_t t);

#endif // CERTICOQ_INT6X_NAT_H