/* Extracted from interp.c:19048 (function primitiveLowSpaceSemaphore). */

/*	Register the low-space semaphore. If the argument is not a
        Semaphore, unregister the current low-space Semaphore. */

/* InterpreterPrimitives>>#primitiveLowSpaceSemaphore */

static void primitiveLowSpaceSemaphore(void) {
  sqInt arg;

  arg = longAt(stackPointer);
  if ((arg == nilObj) || (/* isSemaphoreOop: */
                          ((!(arg & (tagMask())))) &&
                          (((longAt((void *)(arg))) & (classIndexMask())) ==
                           (rawHashBitsOf(fetchPointerofObject(
                               ClassSemaphore, specialObjectsOop)))))) {
    /* begin splObj:put: */
    storePointerofObjectwithValue(TheLowSpaceSemaphore, specialObjectsOop, arg);

    /* begin pop: */
    stackPointer += 1 * BytesPerWord;
  } else {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
  }
}