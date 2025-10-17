/* Extracted from interp.c:37605 (function isRemembered). */

/* SpurMemoryManager>>#isRemembered: */

static NoDbgRegParms int isRemembered(sqInt objOop) {
  return ((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
          (1U << (rememberedBitByteShift()))) != 0;
}