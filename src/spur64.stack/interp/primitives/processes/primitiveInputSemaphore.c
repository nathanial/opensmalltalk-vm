/* Extracted from interp.c:17963 (function primitiveInputSemaphore). */

/*	Register the input semaphore. If the argument is not a
        Semaphore, unregister the current input semaphore. */

/* InterpreterPrimitives>>#primitiveInputSemaphore */

static void primitiveInputSemaphore(void) {
  sqInt arg;

  arg = longAt(stackPointer);
  if ((((arg) & 7) == 1)) {
    ioSetInputSemaphore((arg >> 3));
    if (!primFailCode) {
      /* begin pop: */
      stackPointer += 1 * BytesPerWord;
    }
    return;
  }

  /* If arg is integer, then use it as an index
     into the external objects array and install it
     as the new event semaphore
     old code for compatibility */

  /* begin primitiveFail */
  if (!primFailCode) {
    primFailCode = 1;
  }
  return;
}