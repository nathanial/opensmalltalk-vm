/* Extracted from interp.c:59060 (function
 * printActivationNameForSelectorstartClass). */

/* StackInterpreter>>#printActivationNameForSelector:startClass: */

static NoDbgRegParms void
printActivationNameForSelectorstartClass(sqInt aSelector, sqInt startClass) {
  sqInt classDict;
  usqInt classDictSize;
  sqInt currClass;
  sqInt i;
  sqInt methClass;
  usqInt numSlots;
  sqInt objOop;

  if (addressCouldBeObj(startClass)) {
    /* begin findClassForSelector:lookupClass:do: */
    currClass = startClass;
    do {
      classDict = longAt(
          (void *)((currClass + BaseHeaderSize) +
                   ((((usqInt)(MethodDictionaryIndex) << (shiftForWord()))))));

      /* begin numSlotsOf: */
      assert((classIndexOf(classDict)) > (isForwardedObjectClassIndexPun()));
      classDictSize =
          (((numSlots =
                 byteAt((void *)(classDict + (numSlotsFieldByteOffset()))))) ==
                   (numSlotsMask())
               ? ((((usqInt)(((sqInt)((usqInt)((longAt((
                                          void *)(classDict - BaseHeaderSize))))
                                      << 8)))))) >>
                     8
               : numSlots);
      i = SelectorStart;
      while (i < classDictSize) {
        if (aSelector ==
            (longAt((void *)((classDict + BaseHeaderSize) +
                             ((((usqInt)(i) << (shiftForWord())))))))) {
          methClass = currClass;
          goto l1;
        }
        i += 1;
      }

      /* begin superclassOf: */
      /* begin followObjField:ofObject: */
      objOop =
          longAt((void *)((currClass + BaseHeaderSize) +
                          ((((usqInt)(SuperclassIndex) << (shiftForWord()))))));
      assert(isNonImmediate(objOop));
      if ((!((longAt((void *)(objOop))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        objOop = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex,
                                                          currClass, objOop);
      }
      currClass = objOop;
    } while (!(currClass == nilObj));
    methClass = null;
    /* end findClassForSelector:lookupClass:do: */
  l1:
    if ((!methClass) || (startClass == methClass)) {
      printNameOfClasscount(methClass, 5);
      printChar('>');
      if (!methClass) {
        printStringOf(longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                                      ((((usqInt)(SelectorDoesNotUnderstand)
                                         << (shiftForWord())))))));
        print(" ");
      }
    } else {
      printNameOfClasscount(startClass, 5);
      printChar('(');
      printNameOfClasscount(methClass, 5);
      printChar(')');
      printChar('>');
    }
  } else {
    print("INVALID CLASS");
  }
  if (/* addressCouldBeOop: */
      (((aSelector & (tagMask())) != 0)) || (addressCouldBeObj(aSelector))) {
    if (/* isBytes: */
        ((!(aSelector & (tagMask())))) &&
        (((byteAt((void *)(aSelector + (formatFieldByteOffset())))) &
          (formatMask())) >= (firstByteFormat()))) {
      printStringOf(aSelector);
    } else {
      /* begin printOopShort: */
      printOopShortInner(aSelector);
      fflush(transcript);
    }
  } else {
    print("INVALID SELECTOR");
  }
}