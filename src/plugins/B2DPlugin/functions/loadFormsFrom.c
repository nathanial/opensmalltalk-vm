/* Extracted from B2DPlugin.c:4800 (function loadFormsFrom). */

static sqInt loadFormsFrom(sqInt arrayOop) {
  sqInt bmBits;
  sqInt bmBitsSize;
  sqInt bmDepth;
  sqInt bmHeight;
  sqInt bmRaster;
  sqInt bmWidth;
  sqInt formOop;
  sqInt i;
  sqInt ppw;
  sqInt toDoLimit;

  if (!(isArray(arrayOop))) {
    return 0;
  }
  formArray = arrayOop;
  toDoLimit = (slotSizeOf(formArray)) - 1;
  for (i = 0; i <= toDoLimit; i += 1) {
    formOop = fetchPointerofObject(i, formArray);
    if (!(isPointers(formOop))) {
      return 0;
    }
    if ((slotSizeOf(formOop)) < 5) {
      return 0;
    }
    bmBits = fetchPointerofObject(0, formOop);
    if (!((fetchClassOf(bmBits)) == (classBitmap()))) {
      return 0;
    }
    bmBitsSize = slotSizeOf(bmBits);
    bmWidth = fetchIntegerofObject(1, formOop);
    bmHeight = fetchIntegerofObject(2, formOop);
    bmDepth = fetchIntegerofObject(3, formOop);
    if (failed()) {
      return 0;
    }
    if (!((bmWidth >= 0) && (bmHeight >= 0))) {
      return 0;
    }
    ppw = 32 / bmDepth;
    bmRaster = (bmWidth + (ppw - 1)) / ppw;
    if (!(bmBitsSize == (bmRaster * bmHeight))) {
      return 0;
    }
  }
  return 1;
}