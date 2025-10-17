/* Extracted from interp.c:46845 (function validStorePointerArgs). */

/* SpurMemoryManager>>#validStorePointerArgs:_:_: */

static sqInt validStorePointerArgs(sqInt fieldIndex, sqInt objOop,
                                   sqInt valuePointer) {
  usqInt numSlots;

  return (fieldIndex >= 0) &&
         ((fieldIndex <
           ((/* begin numSlotsOf: */
             assert((classIndexOf(objOop)) >
                    (isForwardedObjectClassIndexPun())),
             (((numSlots =
                    byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) ==
                      (numSlotsMask())
                  ? ((((usqInt)(((sqInt)((usqInt)((longAt((
                                             void *)(objOop - BaseHeaderSize))))
                                         << 8)))))) >>
                        8
                  : numSlots)))) &&
          (!((!((longAt((void *)(objOop))) &
                ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))));
}