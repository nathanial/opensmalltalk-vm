/* Extracted from BitBltPlugin.c:2842 (function fetchIntOrFloatofObjectifNil). */

static sqInt fetchIntOrFloatofObjectifNil(sqInt fieldIndex, sqInt objectPointer,
                                          sqInt defaultValue) {
  sqInt fieldOop;
  double floatValue;

  fieldOop = fetchPointerofObject(fieldIndex, objectPointer);
  if (isIntegerObject(fieldOop)) {
    return integerValueOf(fieldOop);
  }
  if (fieldOop == (nilObject())) {
    return defaultValue;
  }
  floatValue = floatValueOf(fieldOop);
  if (!((-2.147483648e9 <= floatValue) && (floatValue <= 2.147483647e9))) {
    primitiveFail();
    return 0;
  }
  return ((sqInt)floatValue);
}