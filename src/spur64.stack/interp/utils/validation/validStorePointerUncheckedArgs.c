/* Extracted from interp.c:46860 (function validStorePointerUncheckedArgs). */

/* SpurMemoryManager>>#validStorePointerUncheckedArgs:_:_: */

static sqInt validStorePointerUncheckedArgs(sqInt fieldIndex, sqInt objOop,
                                            sqInt valuePointer) {
  usqInt numSlots;

  return (fieldIndex >= 0) &&
         (((fieldIndex == 0) ||
           (fieldIndex <
            ((/* begin numSlotsOf: */
              assert((classIndexOf(objOop)) >
                     (isForwardedObjectClassIndexPun())),
              (((numSlots =
                     byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) ==
                       (numSlotsMask())
                   ? ((((usqInt)((
                         (sqInt)((usqInt)((
                                     longAt((void *)(objOop - BaseHeaderSize))))
                                 << 8)))))) >>
                         8
                   : numSlots))))) &&
          ((((valuePointer & (tagMask())) != 0)) ||
           ((/* isInHeapBounds: */
             (oopisGreaterThanOrEqualTo(valuePointer, newSpaceStart)) &&
             (oopisLessThan(valuePointer, endOfMemory))) ||
            (((fieldIndex == 0) &&
              ((((/* begin numSlotsOf: */
                  assert((classIndexOf(objOop)) >
                         (isForwardedObjectClassIndexPun())),
                  (((numSlots = byteAt(
                         (void *)(objOop + (numSlotsFieldByteOffset()))))) ==
                           (numSlotsMask())
                       ? ((((usqInt)((
                             (sqInt)((usqInt)((longAt(
                                         (void *)(objOop - BaseHeaderSize))))
                                     << 8)))))) >>
                             8
                       : numSlots))) == 0) ||
               (gcPhaseInProgress == SlidingCompactionInProgress))) ||
             ((((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
                (formatMask())) >= (sixtyFourBitIndexableFormat())) &&
              ((((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
                 (formatMask())) < (firstCompiledMethodFormat())) ||
               (fieldIndex > (literalCountOf(objOop)))))))));
}