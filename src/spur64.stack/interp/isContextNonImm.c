/* Extracted from interp.c:37206 (function isContextNonImm). */

/* SpurMemoryManager>>#isContextNonImm: */

static int isContextNonImm(sqInt oop) {
  return ((longAt((void *)(oop))) & (classIndexMask())) ==
         ClassMethodContextCompactIndex;
}