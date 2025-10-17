/* Extracted from interp.c:37213 (function isContext). */

/* SpurMemoryManager>>#isContext: */

static sqInt isContext(sqInt oop) {
  return ((!(oop & (tagMask())))) &&
         (((longAt((void *)(oop))) & (classIndexMask())) ==
          ClassMethodContextCompactIndex);
}