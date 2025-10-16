/* Extracted from interp.c:27356 (function isIntegerValue). */

/*	Answer if the given value can be represented as a Smalltalk integer
   value. In 64-bits we use a 3 bit tag which leaves 61 bits for 2's complement
        signed integers. In C, use a shift add and mask to test if the top 4
   bits are all the same. Since 16rFFFFFFFFFFFFFFFF >> 60 = 16rF the computation
   intValue >> 60 + 1 bitAnd: 16rF maps in-range -ve values to 0 and in-range
   +ve values to 1. */

/* Spur64BitMemoryManager>>#isIntegerValue: */

int isIntegerValue(sqInt intValue) {
  return (((((usqInt)(intValue)) >> 60) + 1) & 15) <= 1;
}