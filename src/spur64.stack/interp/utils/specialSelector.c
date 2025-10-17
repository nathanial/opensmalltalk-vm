/* Extracted from interp.c:63815 (function specialSelector). */

/* StackInterpreter>>#specialSelector: */

sqInt specialSelector(sqInt index) {
  return fetchPointerofObject(index * 2, fetchPointerofObject(SpecialSelectors, specialObjectsOop));
}