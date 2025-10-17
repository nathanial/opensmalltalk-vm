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
    /* begin storePointer:ofObject:withValue: */
    assert(
        validStorePointerArgs(TheInterruptSemaphore, specialObjectsOop, arg));
    assert(isNonImmediate(specialObjectsOop));
    if (oopisGreaterThanOrEqualTo(specialObjectsOop, oldSpaceStart)) {
      if (/* isYoung: */
          ((!(arg & (tagMask())))) && (oopisLessThan(arg, oldSpaceStart))) {
        /* begin possibleRootStoreInto: */
        if (!((byteAt(
                  (void *)(specialObjectsOop + (formatFieldByteOffset())))) &
              (1U << (rememberedBitByteShift())))) {
          remember(specialObjectsOop);
        }
      }
    }

    /* most stores into young objects */
    longAtput(
        (void *)((specialObjectsOop + BaseHeaderSize) +
                 ((((usqInt)(TheInterruptSemaphore) << (shiftForWord()))))),
        arg);

    /* begin pop: */
    stackPointer += 1 * BytesPerWord;
  } else {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
  }
}