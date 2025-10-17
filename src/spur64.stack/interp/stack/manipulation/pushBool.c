/* Extracted from interp.c:61182 (function pushBool). */

/* StackInterpreter>>#pushBool: */

void pushBool(sqInt trueOrFalse) {
  sqInt object;
  char *sp;

  object = booleanObjectOf(trueOrFalse);

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), object);
  stackPointer = sp;
}