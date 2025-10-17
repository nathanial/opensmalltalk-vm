/* Extracted from interp.c:61439 (function quickFetchIntegerofObject). */

/*	Return the integer value of the field without verifying that it is an
        integer value! For use in time-critical places where the integer-ness of
        the field can be guaranteed.
 */

/* StackInterpreter>>#quickFetchInteger:ofObject: */

static sqInt quickFetchIntegerofObject(sqInt fieldIndex, sqInt objectPointer) {
  sqInt oop;

  oop = longAt((void *)((objectPointer + BaseHeaderSize) +
                        ((((usqInt)(fieldIndex) << (shiftForWord()))))));
  assert((((oop) & 7) == 1));
  return (oop >> 3);
}