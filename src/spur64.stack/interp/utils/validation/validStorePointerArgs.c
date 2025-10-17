/* Extracted from interp.c:46845 (function validStorePointerArgs). */

/* SpurMemoryManager>>#validStorePointerArgs:_:_: */

static sqInt validStorePointerArgs(sqInt fieldIndex, sqInt objOop,
                                   sqInt valuePointer) {
  usqInt numSlots;

  return (fieldIndex >= 0) &&
         ((fieldIndex <
           ((
             assert((classIndexOf(objOop)) >
                    (isForwardedObjectClassIndexPun())),
             numSlotsOf(objOop)))) &&
          (!((!((longAt((void *)(objOop))) &
                ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))));
}