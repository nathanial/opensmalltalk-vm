/* Extracted from interp.c:64236 (function stackIntegerValue). */

/*	In the StackInterpreter stacks grow down. */

/* StackInterpreter>>#stackIntegerValue: */

sqInt stackIntegerValue(sqInt offset) {
  sqInt integerPointer;

  integerPointer = longAt(stackPointer + (offset * BytesPerWord));

  /* begin checkedIntegerValueOf: */
  if ((((integerPointer) & 7) == 1)) {
    return (integerPointer >> 3);
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return 0;
  }
}