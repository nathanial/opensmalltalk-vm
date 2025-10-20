/* Extracted from B2DPlugin.c:5299 (function loadSpanBufferFrom). */

static sqInt loadSpanBufferFrom(sqInt spanOop) {
  sqInt value;

  if (!((fetchClassOf(spanOop)) == (classBitmap()))) {
    return GEFClassMismatch;
  }
  spanBuffer = firstIndexableField(spanOop);

  /* Leave last entry unused to avoid complications */
  value = (slotSizeOf(spanOop)) - 1;

  /* begin spanSizePut: */
  workBuffer[GWSpanSize] = value;
  return 0;
}