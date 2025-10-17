/* Extracted from interp.c:58321 (function popthenPush). */

/*	In the StackInterpreter stacks grow down. */

/* StackInterpreter>>#pop:thenPush: */

void popthenPush(sqInt nItems, sqInt oop) {
  char *sp;

  longAtput((sp = stackPointer + ((nItems - 1) * BytesPerWord)), oop);
  stackPointer = sp;
}