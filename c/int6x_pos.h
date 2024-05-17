#ifndef CERTICOQ_INT6X_POS_H
#define CERTICOQ_INT6X_POS_H

// Include the value type
#include <values.h>

// Forward declare the thread info.
extern struct thread_info *tinfo;

value uint63_from_positive(value p);

value uint63_to_positive(struct thread_info *tinfo, value t);

#endif // CERTICOQ_INT6X_POS_H