/* Extracted from interp.c:26186 (function sizeOfAlienData). */

/*	Answer the start of the Alien's data or fail if oop is not an Alien. */

/* InterpreterPrimitives>>#sizeOfAlienData: */

usqInt sizeOfAlienData(sqInt oop) {
  sqInt objOop;
  sqInt oopClass;
  sqInt tagBits;

  /* begin is:KindOfClass: */
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
    if (oopClass ==
        (fetchPointerofObject(ClassAlien, specialObjectsOop))) {
      goto l1;
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

  /* primitiveFailFor: */
  primFailCode = PrimErrBadArgument;
  return 0;
l1:
  return SQABS(longAt((void *)(oop + BaseHeaderSize)));
}