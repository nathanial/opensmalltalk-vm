/* Extracted from interp.c:26228 (function startOfAlienData). */

/*	Answer the start of the Alien's data or fail if oop is not an Alien. */

/* InterpreterPrimitives>>#startOfAlienData: */

void *startOfAlienData(sqInt oop) {
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
    if (oopClass == (fetchPointerofObject(ClassAlien, specialObjectsOop))) {
      goto l1;
    }

    /* begin superclassOf: */
    objOop = followObjFieldofObject(SuperclassIndex, oopClass);
    oopClass = objOop;
  }

  /* primitiveFailFor: */
  primFailCode = PrimErrBadArgument;
  return 0;
l1:
  return (
      (void *)(((longAt((void *)(oop + BaseHeaderSize))) > 0
                    ? (oop + BaseHeaderSize) + BytesPerOop
                    : longAt((void *)((oop + BaseHeaderSize) + BytesPerOop)))));
}