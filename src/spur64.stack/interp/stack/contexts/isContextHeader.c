/* Extracted from interp.c:37199 (function isContextHeader). */

/* SpurMemoryManager>>#isContextHeader: */

static int isContextHeader(sqInt aHeader) {
  return (aHeader & (classIndexMask())) == ClassMethodContextCompactIndex;
}