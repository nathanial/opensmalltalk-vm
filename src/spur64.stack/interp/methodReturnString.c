/* Extracted from interp.c:57851 (function methodReturnString). */

/*	Attempt to answer a ByteString for a given C string as the result of a
        primitive.
 */

/* StackInterpreter>>#methodReturnString: */

NeverInline sqInt methodReturnString(const char *aCString) {
  sqInt result;
  char *sp;

  assert(!((failed())));
  if (aCString) {
    if ((result = stringForCString(aCString))) {
      /* begin pop:thenPush: */
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          result);
      stackPointer = sp;
    } else {
      primFailCode = PrimErrNoMemory;
    }
  } else {
    primFailCode = PrimErrOperationFailed;
  }
  return 0;
}