/* Extracted from interp.c:57800 (function methodReturnFloat). */

/*	Sets the return value for a method. */

/* StackInterpreter>>#methodReturnFloat: */

sqInt methodReturnFloat(double aFloat) {
  assert(!((failed())));

  /* begin pop:thenPushFloat: */
  popthenPushFloat(argumentCount + 1, floatObjectOf(aFloat));
  return 0;
}
