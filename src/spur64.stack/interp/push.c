/* Extracted from interp.c:61240 (function push). */

/*	In the StackInterpreter stacks grow down. */

/* StackInterpreter>>#push: */

void push(sqInt object) {
  char *sp;

  longAtput((sp = stackPointer - BytesPerWord), object);
  stackPointer = sp;
}