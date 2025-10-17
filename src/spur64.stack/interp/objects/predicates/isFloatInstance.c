/* Extracted from interp.c:27322 (function isFloatInstance). */

/* Spur64BitMemoryManager>>#isFloatInstance: */

static int isFloatInstance(sqInt oop) {
  sqInt tagBits;

  return ((tagBits = oop & (tagMask()))
              ? tagBits == (smallFloatTag())
              : ((longAt((void *)(oop))) & (classIndexMask())) ==
                    ClassFloatCompactIndex);
}