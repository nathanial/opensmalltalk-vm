/* Extracted from interp.c:18426 (function primitiveInterruptSemaphore). */

/*	Register the user interrupt semaphore. If the argument is
        not a Semaphore, unregister the current interrupt
        semaphore. */

/* InterpreterPrimitives>>#primitiveInterruptSemaphore */

static void primitiveInterruptSemaphore(void) {
  sqInt arg;

  arg = longAt(stackPointer);
  if ((arg == nilObj) || (/* isSemaphoreOop: */
                          ((!(arg & (tagMask())))) &&
                          (((longAt((void *)(arg))) & (classIndexMask())) ==
                           (rawHashBitsOf(fetchPointerofObject(
                               ClassSemaphore, specialObjectsOop)))))) {
    /* begin splObj:put: */
    storePointerofObjectwithValue(TheInterruptSemaphore, specialObjectsOop, arg);

    /* begin pop: */
    stackPointer += 1 * BytesPerWord;
  } else {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
  }
}