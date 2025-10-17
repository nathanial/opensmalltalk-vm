/* Extracted from interp.c:55504 (function isMemberOf). */

/*	Support for external primitives */

/* StackInterpreter>>#is:MemberOf: */

sqInt isMemberOf(sqInt oop, char *className) {
  sqInt oopClass;
  sqInt tagBits;

  oopClass = /* fetchClassOf: */
      ((tagBits = oop & (tagMask()))
           ? fetchPointerofObject(tagBits, classTableFirstPage)
           : fetchClassOfNonImm(oop));
  return classNameOfIs(oopClass, className);
}