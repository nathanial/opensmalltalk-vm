/* Extracted from interp.c:54959 (function isBooleanObject). */

/* StackInterpreter>>#isBooleanObject: */

sqInt isBooleanObject(sqInt oop) {
  return (oop == trueObj) || (oop == falseObj);
}