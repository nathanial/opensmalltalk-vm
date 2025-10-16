/* Extracted from interp.c:27303 (function integerObjectOf). */

/*	Convert the integer value, assumed to be in SmallInteger range, into a
        tagged SmallInteger object.
        In C, use a shift and an add to set the tag bit.
        In Smalltalk we have to work harder because the simulator works with
        strictly positive bit patterns. */

/* Spur64BitMemoryManager>>#integerObjectOf: */

sqInt integerObjectOf(sqInt value) {
  return (((((usqInt)value)) << (numTagBits()))) + 1;
}