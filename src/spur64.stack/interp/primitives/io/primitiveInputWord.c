/* Extracted from interp.c:17996 (function primitiveInputWord). */

/*	Return an integer indicating the reason for the most recent input
        interrupt.
 */

/* InterpreterPrimitives>>#primitiveInputWord */

static void primitiveInputWord(void) {
  char *sp;

  /* begin pop:thenPushInteger: */
  longAtput((sp = stackPointer), (((usqInt)0 << 3) | 1));
  stackPointer = sp;
}