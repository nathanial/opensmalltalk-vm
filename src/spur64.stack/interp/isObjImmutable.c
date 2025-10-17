/* Extracted from interp.c:37484 (function isObjImmutable). */

/* SpurMemoryManager>>#isObjImmutable: */

static NoDbgRegParms sqInt isObjImmutable(sqInt anOop) {
  return
#if IMMUTABILITY
      ((((usqInt)((
            byteAt((void *)(anOop + (immutableExtraBitsByteOffset())))))) >>
        (immutableBitByteShift())) &
       1) != 0
#else
      0
#endif
      ;
}