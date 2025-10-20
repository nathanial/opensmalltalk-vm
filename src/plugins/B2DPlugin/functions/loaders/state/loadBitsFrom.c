/* Extracted from B2DPlugin.c:4012 (function loadBitsFrom). */

static int *loadBitsFrom(sqInt bmFill) {
  sqInt bitsLen;
  sqInt bitsOop;
  sqInt formOop;
  int xIndex;

  xIndex = objBuffer[bmFill + GEObjectIndex];
  if (xIndex > (slotSizeOf(formArray))) {
    return null;
  }
  formOop = fetchPointerofObject(xIndex, formArray);
  bitsOop = fetchPointerofObject(0, formOop);
  bitsLen = slotSizeOf(bitsOop);
  if (!(bitsLen == (objBuffer[bmFill + GBBitmapSize]))) {
    return null;
  }
  return firstIndexableField(bitsOop);
}