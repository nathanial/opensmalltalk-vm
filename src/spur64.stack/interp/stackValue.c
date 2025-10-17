/* Extracted from interp.c:64469 (function stackValue). */

/*	In the StackInterpreter stacks grow down. */

/* StackInterpreter>>#stackValue: */

sqInt stackValue(sqInt offset) {
  return longAt(stackPointer + (offset * BytesPerWord));
}