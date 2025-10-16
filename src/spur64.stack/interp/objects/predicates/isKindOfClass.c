/* Extracted from interp.c:55436 (function isKindOfClass). */

/*	Support for external primitives. */

/* StackInterpreter>>#is:KindOfClass: */

sqInt isKindOfClass(sqInt oop, sqInt aClass) {
  sqInt objOop;
  sqInt oopClass;
  sqInt tagBits;

  oopClass = /* fetchClassOf: */
      ((tagBits = oop & (tagMask()))
           ? fetchPointerofObject(tagBits, classTableFirstPage)
           : fetchClassOfNonImm(oop));
  while ((oopClass != nilObj) &&
         ((/* isPointers: */
           ((!(oopClass & (tagMask())))) &&
           (((byteAt((void *)(oopClass + (formatFieldByteOffset())))) &
             (formatMask())) <= 5 /* lastPointerFormat */)) &&
          ((numSlotsOfAny(oopClass)) > InstanceSpecificationIndex))) {
    if (oopClass == aClass) {
      return 1;
    }

    /* begin superclassOf: */
    objOop = followObjFieldofObject(SuperclassIndex, oopClass);
    oopClass = objOop;
  }
  return 0;
}