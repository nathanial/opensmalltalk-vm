/* Extracted from BitBltPlugin.c:1762 (function copyBitsRule41Test). */
/* BitBltSimulation>>#copyBitsRule41Test */
/*	Recover from the fast path specialised code saying Help-I-cant-cope */

static sqInt copyBitsRule41Test(void) {
  sqInt gammaLookupTableOop;
  sqInt numArgs;
  sqInt ungammaLookupTableOop;

  numArgs = methodArgumentCount();

  /* fetch the forecolor into componentAlphaModeColor. */
  gammaLookupTable = null;
  ungammaLookupTable = null;
  if (numArgs >= 2) {
    componentAlphaModeAlpha = stackValue(numArgs - 2);
    componentAlphaModeColor = stackValue(numArgs - 1);
    if (!((isIntegerObject(componentAlphaModeAlpha)) &&
          (isIntegerObject(componentAlphaModeColor)))) {
      return 0;
    }
    componentAlphaModeAlpha = integerValueOf(componentAlphaModeAlpha);
    componentAlphaModeColor = integerValueOf(componentAlphaModeColor);
    if (numArgs == 4) {
      gammaLookupTableOop = stackValue(1);
      if (isBytes(gammaLookupTableOop)) {
        gammaLookupTable = firstIndexableField(gammaLookupTableOop);
      }
      ungammaLookupTableOop = stackValue(0);
      if (isBytes(ungammaLookupTableOop)) {
        ungammaLookupTable = firstIndexableField(ungammaLookupTableOop);
      }
    }
    return 1;
  }
  componentAlphaModeAlpha = 0xFF;
  if (numArgs == 1) {
    componentAlphaModeColor = stackValue(0);
    if (!(isIntegerObject(componentAlphaModeColor))) {
      return 0;
    }
    componentAlphaModeColor = integerValueOf(componentAlphaModeColor);
    return 1;
  }
  return 0;
}
