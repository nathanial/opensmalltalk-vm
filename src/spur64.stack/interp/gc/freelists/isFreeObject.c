/* Extracted from interp.c:37311 (function isFreeObject). */

/* SpurMemoryManager>>#isFreeObject: */

static int isFreeObject(sqInt objOop) {
  return ((longAt((void *)(objOop))) & (classIndexMask())) ==
         (isFreeObjectClassIndexPun());
}