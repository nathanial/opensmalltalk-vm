/* Extracted from interp.c:35036 (function fetchClassOf). */

/* SpurMemoryManager>>#fetchClassOf: */

sqInt fetchClassOf(sqInt oop) {
  sqInt tagBits;

  return ((tagBits = oop & (tagMask()))
              ? fetchPointerofObject(tagBits, classTableFirstPage)
              : fetchClassOfNonImm(oop));
}