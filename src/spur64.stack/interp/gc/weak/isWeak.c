/* Extracted from interp.c:37844 (function isWeak). */

/*	Answer if the argument has only weak fields that can hold oops. See
        comment in formatOf:
 */

/* SpurMemoryManager>>#isWeak: */

sqInt isWeak(sqInt oop) {
  return ((!(oop & (tagMask())))) && (isWeakNonImm(oop));
}