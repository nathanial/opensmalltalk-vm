/* Extracted from interp.c:57851 (function methodReturnString). */

/*	Attempt to answer a ByteString for a given C string as the result of a
        primitive.
 */

/* StackInterpreter>>#methodReturnString: */

NeverInline sqInt methodReturnString(const char *aCString) {
  sqInt result;

  assert(!((failed())));
  if (aCString) {
    if ((result = stringForCString(aCString))) {
      /* begin pop:thenPush: */
      popthenPush(argumentCount + 1, result);
    } else {
      primFailCode = PrimErrNoMemory;
    }
  } else {
    primFailCode = PrimErrOperationFailed;
  }
  return 0;
}
