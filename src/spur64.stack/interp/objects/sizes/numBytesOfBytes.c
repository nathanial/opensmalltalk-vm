/* Extracted from interp.c:41075 (function numBytesOfBytes). */

/*	Answer the number of indexable bytes in the given non-immediate
        byte-indexable object.
 */

/* SpurMemoryManager>>#numBytesOfBytes: */

static sqInt numBytesOfBytes(sqInt objOop) {
  sqInt fmt;
  usqInt numSlots;

  fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
  assert(fmt >= (firstByteFormat()));
  return ((((assert((classIndexOf(objOop)) >
                    (isForwardedObjectClassIndexPun())),
             numSlotsOf(objOop)))
           << (shiftForWord()))) -
         (fmt & 7);
}