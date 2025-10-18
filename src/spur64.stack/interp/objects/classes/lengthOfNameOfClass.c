/* Extracted from interp.c:55518 (function lengthOfNameOfClass). */

/* StackInterpreter>>#lengthOfNameOfClass: */

static sqInt lengthOfNameOfClass(sqInt classOop) {
  usqInt numSlots;
  sqInt objOop;

  numSlots = numSlotsOf(classOop);
  if (numSlots == metaclassNumSlots) {
    return lengthOfNameOfClass(fetchPointerofObject(thisClassIndex, classOop));
  }
  if (numSlots <= classNameIndex) {
    return 0;
  }
  objOop = fetchPointerofObject(classNameIndex, classOop);

  return lengthOf(objOop);
}
