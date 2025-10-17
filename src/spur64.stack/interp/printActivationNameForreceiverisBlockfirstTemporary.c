/* Extracted from interp.c:59143 (function
 * printActivationNameForreceiverisBlockfirstTemporary). */

/* StackInterpreter>>#printActivationNameFor:receiver:isBlock:firstTemporary: */

static NoDbgRegParms void printActivationNameForreceiverisBlockfirstTemporary(
    sqInt aMethod, sqInt anObject, sqInt isBlock, sqInt maybeMessage) {
  sqInt classObj;
  sqInt homeMethod;
  sqInt methClass;
  sqInt methodSel;
  sqInt tagBits;

  if (isBlock) {
    print("[] in ");
    homeMethod = homeMethodOf(aMethod);
  } else {
    homeMethod = aMethod;
  }
  methClass = findClassOfMethodforReceiver(homeMethod, anObject);
  methodSel = findSelectorOfMethod(homeMethod);
  if ((/* addressCouldBeOop: */
       (((anObject & (tagMask())) != 0)) || (addressCouldBeObj(anObject))) &&
      ((!(/* isOopForwarded: */
          ((!(anObject & (tagMask())))) &&
          ((!((longAt((void *)(anObject))) &
              ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))) &&
       (addressCouldBeClassObj(
           (classObj = /* fetchClassOf: */
            ((tagBits = anObject & (tagMask()))
                 ? longAt((void *)((classTableFirstPage + BaseHeaderSize) +
                                   ((((usqInt)(tagBits) << (shiftForWord()))))))
                 : fetchClassOfNonImm(anObject))))))) {
    printNameOfClasscount(classObj, 5);
    if (!((classObj == methClass) || ((!methClass) || (methClass == nilObj)))) {
      printChar('(');
      printNameOfClasscount(methClass, 5);
      printChar(')');
    }
  } else {
    print("INVALID RECEIVER");
  }
  printChar('>');
  if (/* addressCouldBeOop: */
      (((methodSel & (tagMask())) != 0)) || (addressCouldBeObj(methodSel))) {
    if (methodSel == nilObj) {
      print("(nil)");
    } else {
      if (((methodSel & (tagMask())) != 0)) {
        printChar('(');
        printHex(methodSel);
        printChar(')');
      } else {
        printStringOf(methodSel);
      }
    }
  } else {
    print("INVALID SELECTOR = ");
    printHex(methodSel);
  }
  if ((methodSel == (longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                                     ((((usqInt)(SelectorDoesNotUnderstand)
                                        << (shiftForWord())))))))) &&
      ((addressCouldBeObj(maybeMessage)) &&
       ((fetchClassOfNonImm(maybeMessage)) ==
        (longAt(
            (void *)((specialObjectsOop + BaseHeaderSize) +
                     ((((usqInt)(ClassMessage) << (shiftForWord())))))))))) {
    methodSel = longAt(
        (void *)((maybeMessage + BaseHeaderSize) +
                 ((((usqInt)(MessageSelectorIndex) << (shiftForWord()))))));

    /* begin space */
    printChar(' ');
    printStringOf(methodSel);
  }
}