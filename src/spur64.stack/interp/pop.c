/* Extracted from interp.c:58310 (function pop). */

/*	In the StackInterpreter stacks grow down. */

/* StackInterpreter>>#pop: */

sqInt pop(sqInt nItems) {
  stackPointer += nItems * BytesPerWord;
  return null;
}