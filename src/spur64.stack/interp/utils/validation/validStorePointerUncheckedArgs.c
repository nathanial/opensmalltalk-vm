/* Extracted from interp.c:46860 (function validStorePointerUncheckedArgs). */

/* SpurMemoryManager>>#validStorePointerUncheckedArgs:_:_: */

static sqInt validStorePointerUncheckedArgs(sqInt fieldIndex, sqInt objOop,
                                            sqInt valuePointer) {
  usqInt numSlots;

  return (fieldIndex >= 0) &&
         (((fieldIndex == 0) ||
           (fieldIndex < ((assert((classIndexOf(objOop)) >
                                  (isForwardedObjectClassIndexPun())),
                           numSlotsOf(objOop))))) &&
          ((((valuePointer & (tagMask())) != 0)) ||
           ((/* isInHeapBounds: */
             (oopisGreaterThanOrEqualTo(valuePointer, newSpaceStart)) &&
             (oopisLessThan(valuePointer, endOfMemory))) ||
            (((fieldIndex == 0) &&
              ((((assert((classIndexOf(objOop)) >
                         (isForwardedObjectClassIndexPun())),
                  numSlotsOf(objOop))) == 0) ||
               (gcPhaseInProgress == SlidingCompactionInProgress))) ||
             ((((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
                (formatMask())) >= (sixtyFourBitIndexableFormat())) &&
              ((((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
                 (formatMask())) < (firstCompiledMethodFormat())) ||
               (fieldIndex > (literalCountOf(objOop)))))))));
}