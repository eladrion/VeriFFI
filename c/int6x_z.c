/*
 * This file contains primitives for transformation between Z and int64_t.
 *
 * Most parts are taken from the file
 *  examples/uint63z/prims.c
 * Copyright belongs to the respective authors.
 *
 * The function uint63_to_Z was split to extrapolate uint63_to_pos
 * and than reuse that in int64_to_z if the value to transform is
 * negative.
 */

// Include the prototypes of the functions defined here
#include <int6x_z.h>
// Include the prototypes of functions for transforming between int64_t/uint63
#include <int64.h>
// Include math library for abs().
#include <certicoq_math.h>
// Include glue, e.g. for using make_Coq_Numbers_BinNums_Z_Z0
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

typedef enum { Z0, ZPOS, ZNEG } tag_Z;
value uint63_from_Z(value z) {
  switch (get_Coq_Numbers_BinNums_Z_tag(z)) {
    case Z0:
      return 0;
    case ZPOS:
      return uint63_from_positive(get_args(z)[0]);
    case ZNEG:
      return (value)(-(size_t)(uint63_from_positive(get_args(z)[0])));
  }
}

inline int64_t int64_from_Z(value z)
{
  return decode_int63(uint63_from_Z(z));
}

value uint63_to_pos(struct thread_info *tinfo, value t) {
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

value uint63_to_Z(struct thread_info *tinfo, value t) {
  if (t == 1) {
    return make_Coq_Numbers_BinNums_Z_Z0();
  }
  // t can be only positive
  value temp = uint63_to_pos(tinfo, t);

  return alloc_make_Coq_Numbers_BinNums_Z_Zpos(tinfo, temp);
}

value int64_to_Z(struct thread_info *tinfo, int64_t t)
{
  if (t < 0)
  { // t is negative -> remove the sign and create (ZNeg positive)
    // TODO: What if t = –2^63
    value temp = uint63_to_pos(tinfo, encode_int63(abs(t)));
    return alloc_make_Coq_Numbers_BinNums_Z_Zneg(tinfo, temp);
  }
  else
  {
    return uint63_to_Z(tinfo, encode_int63(t));
  }
}