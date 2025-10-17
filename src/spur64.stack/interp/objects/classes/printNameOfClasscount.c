/* Extracted from interp.c:60311 (function printNameOfClasscount). */

/*	Details: The count argument is used to avoid a possible infinite
   recursion if classOop is a corrupted object.
 */

/* StackInterpreter>>#printNameOfClass:count: */

static void printNameOfClasscount(sqInt classOop, sqInt cnt) {
  usqInt numSlots;

  if (!classNameIndex) {
    print("??nil cnidx??");
    return;
  }
  if ((!classOop) || ((classOop == 0) || (cnt <= 0))) {
    print("bad class");
    return;
  }
  numSlots = numSlotsOf(classOop);
  if ((numSlots == metaclassNumSlots) && (metaclassNumSlots > thisClassIndex)) {
    printNameOfClasscount(fetchPointerofObject(thisClassIndex, classOop),
                          cnt - 1);
    print(" class");
  } else {
    if (numSlots <= classNameIndex) {
      print("bad class");
    } else {
      printStringOf(fetchPointerofObject(classNameIndex, classOop));
    }
  }
}