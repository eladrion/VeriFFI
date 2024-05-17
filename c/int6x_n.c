/*
 * This file contains primitives for transformation between N and uint64_t.
 *
 * The functionality is inspired by
 *  examples/uint63z/prims.c
 * and reuses the transformations from and to positive numbers.
 *
 */

// Include the prototypes of the functions defined here
#include <int6x_n.h>
// Include the prototypes of the the transformation functions for positive numbers
#include <int6x_pos.h>
// Include the prototypes of functions for transforming between int64_t/uint63
#include <int64.h>
// Include glue, e.g. for using make_Coq_Numbers_BinNums_N_N0
#include <glue.h>


typedef enum { N0, NPOS } tag_N;
value uint63_from_N(value n) {
  switch (get_Coq_Numbers_BinNums_N_tag(n)) {
    case N0:
      return 0;
    case NPOS:
      return uint63_from_positive(get_args(n)[0]);
  }
}

inline uint64_t uint64_from_N(value n)
{
  return decode_int63((uint64_t) uint63_from_N(n));
}


value uint63_to_N(struct thread_info *tinfo, value t) {
  if (t == 1) {
    return make_Coq_Numbers_BinNums_N_N0();
  }
  // t can be only positive
  value temp = uint63_to_positive(tinfo, t);

  return alloc_make_Coq_Numbers_BinNums_N_Npos(tinfo, temp);
}

value uint64_to_N(struct thread_info *tinfo, uint64_t t)
{
  return uint63_to_N(tinfo, encode_int63(t));
}