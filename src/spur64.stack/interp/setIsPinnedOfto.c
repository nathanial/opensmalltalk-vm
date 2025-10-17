/* Extracted from interp.c:45519 (function setIsPinnedOfto). */

/* SpurMemoryManager>>#setIsPinnedOf:to: */

static NoDbgRegParms void setIsPinnedOfto(sqInt objOop, sqInt aBoolean) {
  assert((isOldObject(objOop)) && (!(isFreeObject(objOop))));
  byteAtput((void *)(objOop + (formatFieldByteOffset())),
            (aBoolean ? (byteAt((void *)(objOop + (formatFieldByteOffset())))) |
                            (1U << (pinnedBitByteShift()))
                      : (byteAt((void *)(objOop + (formatFieldByteOffset())))) &
                            (0xFF - (1U << (pinnedBitByteShift())))));
}