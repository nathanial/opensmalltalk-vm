/* Extracted from interp.c:57879 (function methodReturnValue). */

/*	Sets the return value for a method. In the CoInterpreter we replace the
        cumbersome primResult machinery. */

/* StackInterpreter>>#methodReturnValue: */

sqInt methodReturnValue(sqInt oop) {

  assert(!((failed())));

  /* begin pop:thenPush: */
  popthenPush(argumentCount + 1, oop);
  return 0;
}
