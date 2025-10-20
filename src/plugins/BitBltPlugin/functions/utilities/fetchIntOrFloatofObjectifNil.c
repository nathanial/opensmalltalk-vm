/* Extracted from BitBltPlugin.c:2842 (function fetchIntOrFloatofObjectifNil). */
/* BitBltSimulation>>#fetchIntOrFloat:ofObject:ifNil: */
/*	For any non-zero pixel value in destinationWord with zero alpha channel
        take the alpha from sourceWord and fill it in. Intended for fixing alpha
        channels left at zero during 16->32 bpp conversions.
 */

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
