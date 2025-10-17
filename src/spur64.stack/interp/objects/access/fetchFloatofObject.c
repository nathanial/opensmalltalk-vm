/* Extracted from interp.c:52964 (function fetchFloatofObject). */

/*	Fetch the instance variable at the given index of the given object.
   Return the C double precision floating point value of that instance variable,
   or fail if it is not a Float.
 */
/*	Note: May be called by translated primitive code. */

/* StackInterpreter>>#fetchFloat:ofObject: */

double fetchFloatofObject(sqInt fieldIndex, sqInt objectPointer) {
  sqInt floatOop;

  floatOop = longAt((void *)((objectPointer + BaseHeaderSize) +
                             ((((usqInt)(fieldIndex) << (shiftForWord()))))));
  return floatValueOf(floatOop);
}