/* Extracted from interp.c:22576 (function primitiveSignalAtBytesLeft). */

/*	Set the low-water mark for free space. When the free space falls
        below this level, the new and new: primitives fail and system attempts
        to allocate space (e.g., to create a method context) cause the low-space
        semaphore (if one is registered) to be signalled. */

/* InterpreterPrimitives>>#primitiveSignalAtBytesLeft */

static void primitiveSignalAtBytesLeft(void) {
  sqInt bytes;

  bytes = longAt(stackPointer);
  if (((((bytes) & 7) == 1)) && (((bytes = (bytes >> 3))) >= 0)) {
    /* begin lowSpaceThreshold: */
    lowSpaceThreshold = bytes;

    /* N.B. The threshold > 0 guard eliminates a warning when
       self lowSpaceThreshold: 0
       is inlined into setSignalLowSpaceFlagAndSaveProcess */
    if ((bytes > 0) && (totalFreeOldSpace < bytes)) {
      growOldSpaceByAtLeast(bytes - totalFreeOldSpace);
    }
    assert(totalFreeOldSpace >= lowSpaceThreshold);

    /* begin pop: */
    stackPointer += 1 * BytesPerWord;
  } else {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
  }
}