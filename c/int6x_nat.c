/*
 * This file contains primitives for transformation between nat and
 * int64_t/uint63.
 *
 * The transformations from/to uint63 including the beginframe, livepointer and
 * gc_save macros are taken from the file
 *   examples/uint63nat/prims.c
 * Copyright belongs to the respective authors.
 */

// Include the prototypes of the functions defined here
#include <int6x_nat.h>
// Include the prototypes of functions for transforming between int64_t/int63
#include <int64.h>
// Include GC for calls to garbage_collect, for example.
#include <gc_stack.h>
// Include the glue which for ex. includes get_Coq_Init_Datatypes_nat_tag
#include <glue.h>

/* THE FOLLOWING SHOULD EVENTUALLY BE MOVED INTO gc_stack.h */
/* BEGINFRAME / LIVEPOINTERSn / ENDFRAME
  Usage:

value myfunc(struct thread_info *tinfo, ...other args...) {
  ... some variable declarations ...
  BEGINFRAME(tinfo,n)
  ... some more variable declarations ...

  ...
  r=LIVEPOINTERSj(tinfo,funcallx,a0,a1,...,aj-1);    [where j<n]
  ...
  LIVEPOINTERSk(tinfo,(funcally,NULL),a0,a1,...,ak-1);    [where k<n]


  ENDFRAME
}

  The "tinfo" argument to BEGINFRAME is the tinfo pointer of the
  enclosing function.  The number n is the maximum frame size, that is,
  the max number of pointervalues to save across any call within the
  BEGINFRAME/ENDFRAME block.  There may be zero or more calls to
  LIVEPOINTERS[0,1,2,3,etc] within the block.  In each such call,
  the "funcall" is some expression that calls a function, returning
  a result of type "value", and the arguments ai are pointer values
  to save across the call.  The result of calling the function will
  be returned as the result of LIVEPOINTERS; in the pattern shown,
  it would be put into "r".
     To call a void-returning function f(x), then use  (f(x),(value)NULL)
  as the funcall argument, and in that case you may leave out
  the r=  in the pattern shown.
     It's important that the implementation of ENDFRAME has no
  executable code, because it may be bypassed by a function return.
*/

#define BEGINFRAME(tinfo,n) {{{{{ \
   value *_ALLOC, *_LIMIT; \
   value __ROOT__[n];   \
   struct stack_frame __FRAME__ = { NULL/*bogus*/, __ROOT__, tinfo->fp }; \
   struct stack_frame *__PREV__; \
   size_t nalloc; \
   value __RTEMP__;

#define ENDFRAME }}}}}

#define LIVEPOINTERS0(tinfo, exp) (exp)

#define LIVEPOINTERS1(tinfo, exp, a0) \
   (tinfo->fp= &__FRAME__, __FRAME__.next=__ROOT__+1, \
   __ROOT__[0]=(a0), __RTEMP__=(exp), (a0)=__ROOT__[0], \
   __PREV__=__FRAME__.prev, tinfo->fp=__PREV__, __RTEMP__)

#define LIVEPOINTERS2(tinfo, exp, a0, a1)	\
  (tinfo->fp= &__FRAME__, __FRAME__.next=__ROOT__+2, \
  __ROOT__[0]=(a0), __ROOT__[1]=(a1),		\
  __RTEMP__=(exp),                              \
  (a0)=__ROOT__[0], (a1)=__ROOT__[1],             \
   __PREV__=__FRAME__.prev, tinfo->fp=__PREV__, __RTEMP__)

#define LIVEPOINTERS3(tinfo, exp, a0, a1, a2)   \
  (tinfo->fp= &__FRAME__, __FRAME__.next=__ROOT__+3,                       \
  __ROOT__[0]=(a0), __ROOT__[1]=(a1), __ROOT__[2]=(a2),  \
  __RTEMP__=(exp),                                       \
  (a0)=__ROOT__[0], (a1)=__ROOT__[1], (a2)=__ROOT__[2],    \
   __PREV__=__FRAME__.prev, tinfo->fp=__PREV__, __RTEMP__)

#define LIVEPOINTERS4(tinfo, exp, a0, a1, a2, a3)	\
  (tinfo->fp= &__FRAME__,  __FRAME__.next=__ROOT__+4,  \
  __ROOT__[0]=(a0), __ROOT__[1]=(a1), __ROOT__[2]=(a2), __ROOT__[3]=(a3),  \
  __RTEMP__=(exp),                                       \
  (a0)=__ROOT__[0], (a1)=__ROOT__[1], (a2)=__ROOT__[2], (a3)=__ROOT__[3],    \
   __PREV__=__FRAME__.prev, tinfo->fp=__PREV__, __RTEMP__)
/* END OF STUFF TO BE MOVED INTO gc_stack.h */

typedef enum { O, S } nat;

value uint63_from_nat(value n) {
  value temp = n;
  uint64_t i = 0;

  while (get_Coq_Init_Datatypes_nat_tag(temp) == S) {
    i++;
    temp = get_args(temp)[0];
  }
  return (value) ((i << 1) + 1);
}

inline int64_t int64_from_nat(value n)
{
  return decode_int63(uint63_from_nat(n));
}

/* Usage of GC_SAVE1
  Before the invocation of GC_SAVE1(n),
      the variable __FRAME__ (etc) must be set up as by BEGINFRAME(tinfo,k) with k>=n

  After the invocation of GC_SAVE1(n),
     n <= tinfo->limit-tinfo->alloc
  AND  the possibly pointer variable  save0 will have properly forwarded
  AND  all the pointers in the stack-of-frames (from tinfo->fp) will have been forwarded
  AND  no other pointer variables are correctly preserved
  AND  all other nonpointer variables are preserved (except _LIMIT, _ALLOC)

  We cannot allow the name of variable save0 to be a parameter to this macro,
  it must be named exactly that for the convenience of Lemma semax_GC_SAVE1.
 */
#define GC_SAVE1 \
    if (!(_LIMIT=tinfo->limit, _ALLOC=tinfo->alloc, nalloc <= _LIMIT-_ALLOC)) { \
    tinfo->nalloc = nalloc;  \
    LIVEPOINTERS1(tinfo,(garbage_collect(tinfo),(value)NULL),save0);	\
  }

#define GC_SAVE2 \
    if (!(_LIMIT=tinfo->limit, _ALLOC=tinfo->alloc, nalloc <= _LIMIT-_ALLOC)) { \
    tinfo->nalloc = nalloc;  \
    LIVEPOINTERS2(tinfo,(garbage_collect(tinfo),(value)NULL),save0,save1);  \
  }

value uint63_to_nat(struct thread_info *tinfo, value t) {
  uint64_t i = (uint64_t) (((uint64_t) t) >> 1);
  value save0 = make_Coq_Init_Datatypes_nat_O(); /* must name this save0 for compatibility with GC_SAVE1 */
  BEGINFRAME(tinfo,1)
  while (i) {
    nalloc=2; GC_SAVE1  /* no semicolon! */
    save0 = alloc_make_Coq_Init_Datatypes_nat_S(tinfo,save0);
    i--;
  }
  return save0;
  ENDFRAME
}

inline value int64_to_nat(struct thread_info *tinfo, int64_t t)
{
  return uint63_to_nat(tinfo, encode_int63(t));
}