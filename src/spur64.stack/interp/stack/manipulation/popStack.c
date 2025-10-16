/* Extracted from interp.c:58296 (function popStack). */

/*	In the StackInterpreter stacks grow down. */

/* StackInterpreter>>#popStack */

sqInt popStack(void) {
  sqInt top;

  top = longAt(stackPointer);
  stackPointer += BytesPerWord;
  return top;
}