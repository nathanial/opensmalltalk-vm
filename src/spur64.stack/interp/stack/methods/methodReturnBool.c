/* Extracted from interp.c:57780 (function methodReturnBool). */

/*	Sets the return value for a method. In the CoInterpreter we replace the
        cumbersome primResult machinery. */

/* StackInterpreter>>#methodReturnBool: */

sqInt methodReturnBool(sqInt boolean) {

  assert(!((failed())));

  /* begin pop:thenPushBool: */
  popthenPushBool(argumentCount + 1, booleanObjectOf(boolean));
  return 0;
}