/* Extracted from interp.c:59060 (function
 * printActivationNameForSelectorstartClass). */

/* StackInterpreter>>#printActivationNameForSelector:startClass: */

static void printActivationNameForSelectorstartClass(sqInt aSelector,
                                                     sqInt startClass) {
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
      classDict = fetchPointerofObject(MethodDictionaryIndex, currClass);

      
      classDictSize =
          numSlotsOf(classDict);
      i = SelectorStart;
      while (i < classDictSize) {
        if (aSelector == (fetchPointerofObject(i, classDict))) {
          methClass = currClass;
          goto l1;
        }
        i += 1;
      }

      /* begin superclassOf: */
      /* begin followObjField:ofObject: */
      objOop = fetchPointerofObject(SuperclassIndex, currClass);
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
        printStringOf(
            fetchPointerofObject(SelectorDoesNotUnderstand, specialObjectsOop));
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