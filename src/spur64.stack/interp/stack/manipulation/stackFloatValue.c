/* Extracted from interp.c:64226 (function stackFloatValue). */

/*	In the StackInterpreter stacks grow down. */

/* StackInterpreter>>#stackFloatValue: */

double stackFloatValue(sqInt offset) {
  return floatValueOf(longAt(stackPointer + (offset * BytesPerWord)));
}