/* Extracted from interp.c:55472 (function isKindOf). */

/*	Support for external primitives. */

/* StackInterpreter>>#is:KindOf: */

sqInt isKindOf(sqInt oop, char *className) {
  sqInt objOop;
  sqInt oopClass;
  sqInt tagBits;

  oopClass = /* fetchClassOf: */
      ((tagBits = oop & (tagMask()))
           ? fetchPointerofObject(tagBits, classTableFirstPage)
           : fetchClassOfNonImm(oop));
  while (!(oopClass == nilObj)) {
    if (classNameOfIs(oopClass, className)) {
      return 1;
    }

    /* begin superclassOf: */
    /* begin followObjField:ofObject: */
    objOop =
        fetchPointerofObject(SuperclassIndex, oopClass);
    assert(isNonImmediate(objOop));
    if ((!((longAt((void *)(objOop))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
      objOop = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex,
                                                        oopClass, objOop);
    }
    oopClass = objOop;
  }
  return 0;
}