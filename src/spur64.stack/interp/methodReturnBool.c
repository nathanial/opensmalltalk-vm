/* Extracted from interp.c:57780 (function methodReturnBool). */

/*	Sets the return value for a method. In the CoInterpreter we replace the
        cumbersome primResult machinery. */

/* StackInterpreter>>#methodReturnBool: */

sqInt methodReturnBool(sqInt boolean) {
  char *sp;

  assert(!((failed())));

  /* begin pop:thenPushBool: */
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) *
                                  BytesPerWord)), /* booleanObjectOf: */
            (boolean ? trueObj : falseObj));
  stackPointer = sp;
  return 0;
}