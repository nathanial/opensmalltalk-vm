/* Extracted from interp.c:29627 (function activeAndDeferredScan). */

/*	Answer whether an ephemeron is active (has an unmarked
        key) and was pushed on the unscanned ephemerons stack. */

/* SpurMemoryManager>>#activeAndDeferredScan: */

static NeverInline sqInt activeAndDeferredScan(sqInt anEphemeron) {
  sqInt key;

  key = 0;
  assert(isEphemeron(anEphemeron));
  if ((isImmediate((/* begin keyOfEphemeron: */
                    assert((isNonImmediate(anEphemeron)) &&
                           (isObjEphemeron(anEphemeron))),
                    (key = fetchPointerofObject(0U, anEphemeron))))) ||
      (((byteAt((void *)(key + (markBitsByteOffset())))) &
        (1U << (markedBitByteShift()))) != 0)) {
    return 0;
  }
  return pushOnUnscannedEphemeronsStack(anEphemeron);
}