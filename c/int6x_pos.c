/*
 * This file contains primitives for transformation between Z and int64_t.
 *
 * Most parts are taken from the file
 *  examples/uint63z/prims.c
 * Copyright belongs to the respective authors.
 *
 * The function uint63_to_Z was split to extrapolate uint63_to_positive
 * and than reuse that in int64_to_z if the value to transform is
 * negative.
 */

// Include the prototypes of the functions defined here
#include <int6x_pos.h>
// Include standard integers since we use size_t.
#include <certicoq_stdint.h>
// Include glue, e.g. for using alloc_make_Coq_Numbers_BinNums_positive_xI
#include <glue.h>


typedef enum { XI, XO, XH } tag_positive;
// not very space efficient but it should be easy to prove
value uint63_from_positive(value p) {
  switch (get_Coq_Numbers_BinNums_positive_tag(p)) {
    case XI:
      return ((2 * ((size_t)(uint63_from_positive(get_args(p)[0])) >> 1) + 1) << 1) + 1;
    case XO:
      return ((2 * ((size_t)(uint63_from_positive(get_args(p)[0])) >> 1)) << 1) + 1;
    case XH:
      return (1 << 1) + 1;
  }
}


value uint63_to_positive(struct thread_info *tinfo, value t) {
  value temp = 0;
  // loop over bits from left (most significant) to right (least significant)
  // ignore the last bit, hence i > 0, not i >= 0
  for (unsigned int i = sizeof(value) * 8 - 1; i > 0; i--) {
    _Bool bit = (t & (1LLU << i)) >> i;
    if (bit) {
      if (temp) {
        temp = alloc_make_Coq_Numbers_BinNums_positive_xI(tinfo, temp);
      } else {
        temp = make_Coq_Numbers_BinNums_positive_xH();
      }
    } else if (temp) {
      temp = alloc_make_Coq_Numbers_BinNums_positive_xO(tinfo, temp);
    }
    // ignore the 0 bits before the first significant 1
  }
  // Return the positive value.
  return temp;
}

