/* Extracted from interp.c:31094 (function allUnscannedEphemeronsAreActive). */

/* SpurMemoryManager>>#allUnscannedEphemeronsAreActive */

static sqInt allUnscannedEphemeronsAreActive(void) {
  sqInt key;
  usqInt p;

  for (p = ((unscannedEphemerons.start)); p <= ((unscannedEphemerons.top));
       p += BytesPerOop) {
    /* begin keyOfMaybeFiredEphemeron: */
    assert((isNonImmediate(longAt((void *)(p)))) &&
           (isMaybeFiredEphemeron(longAt((void *)(p)))));
    key = fetchPointerofObject(0U, longAt((void *)(p)));
    if ((((key & (tagMask())) != 0)) ||
        (((byteAt((void *)(key + (markBitsByteOffset())))) &
          (1U << (markedBitByteShift()))) != 0)) {
      return 0;
    }
  }
  return 1;
}