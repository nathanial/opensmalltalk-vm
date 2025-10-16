/* Extracted from interp.c:37527 (function isOopImmutable). */

/* SpurMemoryManager>>#isOopImmutable: */

sqInt isOopImmutable(sqInt oop) {
  return (((oop & (tagMask())) != 0)) ||
         (((((usqInt)((
                byteAt((void *)(oop + (immutableExtraBitsByteOffset())))))) >>
            (immutableBitByteShift())) &
           1) != 0);
}