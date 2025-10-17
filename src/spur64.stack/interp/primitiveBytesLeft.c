/* Extracted from interp.c:13236 (function primitiveBytesLeft). */

/*	Answer bytes available at this moment. For more meaningful
        results, calls to this primitive should be precedeed by a full
        or incremental garbage collection. */

/* InterpreterPrimitives>>#primitiveBytesLeft */

static void primitiveBytesLeft(void) {
  sqInt aBool;
  sqInt chunk;
  sqInt freeChunk;
  sqInt i;
  sqInt integerVal;
  char *sp;

  if (!argumentCount) {
    /* begin pop:thenPushInteger: */
    longAtput((sp = stackPointer), ((totalFreeOldSpace << 3) | 1));
    stackPointer = sp;
    return;
  }

  /* old behavior - just return the size of free memory */
  if (argumentCount == 1) {
    if ((longAt(stackPointer)) == nilObj) {
      /* begin sizeOfLargestFreeChunk */
      freeChunk = findLargestFreeChunk();
      if (!freeChunk) {
        for (i = 0x3F; i >= 1; i += -1) {
          if ((chunk = freeLists[i])) {
            integerVal = bytesInBody(chunk);
            goto l1;
          }
        }
        integerVal = 0;
        goto l1;
      }
      integerVal = bytesInBody(freeChunk);
      /* end sizeOfLargestFreeChunk */
    l1:

      /* begin pop:thenPushInteger: */
      longAtput((sp = stackPointer + (1 * BytesPerWord)),
                (((usqInt)integerVal << 3) | 1));
      stackPointer = sp;
      return;
    }

    /* new behaviour -including or excluding swap space depending on aBool */

    /* begin booleanValueOf: */
    if ((longAt(stackPointer)) == trueObj) {
      aBool = 1;
      goto l2;
    }
    if ((longAt(stackPointer)) == falseObj) {
      aBool = 0;
      goto l2;
    }

    /* begin success: */
    if (!primFailCode) {
      primFailCode = 1;
    }
    aBool = null;
    /* end booleanValueOf: */
  l2:
    if (!primFailCode) {
      integerVal =
          ((((totalFreeOldSpace + (scavengeThreshold - freeStart)) -
             (interpreterAllocationReserveBytes())) < 0)
               ? 0
               : ((totalFreeOldSpace + (scavengeThreshold - freeStart)) -
                  (interpreterAllocationReserveBytes())));

      /* begin pop:thenPushInteger: */
      longAtput((sp = stackPointer + (1 * BytesPerWord)),
                (((usqInt)integerVal << 3) | 1));
      stackPointer = sp;
      return;
    }
  }

  /* Spur behavior; if argument is nil answer size of largest free chunk in
   * oldSpace. */

  /* begin primitiveFail */
  if (!primFailCode) {
    primFailCode = 1;
  }
  return;
}