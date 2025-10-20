/* Extracted from B2DPlugin.c:5562 (function loadWorkBufferFrom). */

static sqInt loadWorkBufferFrom(sqInt wbOop) {
  if (isImmediate(wbOop)) {
    return GEFWorkBufferIsInteger;
  }
  if (!(isWords(wbOop))) {
    return GEFWorkBufferIsPointers;
  }
  if ((slotSizeOf(wbOop)) < GWMinimalSize) {
    return GEFWorkBufferTooSmall;
  }

  /* begin workBufferPut: */
  workBuffer = firstIndexableField(wbOop);
  if (!((workBuffer[GWMagicIndex]) == GWMagicNumber)) {
    return GEFWorkBufferBadMagic;
  }

  /* Sanity checks */
  if (!((workBuffer[GWSize]) == (slotSizeOf(wbOop)))) {
    return GEFWorkBufferWrongSize;
  }
  if (!((workBuffer[GWObjStart]) == GWHeaderSize)) {
    return GEFWorkBufferStartWrong;
  }

  /* Load buffers */
  objBuffer = workBuffer + (workBuffer[GWObjStart]);
  getBuffer = objBuffer + (workBuffer[GWObjUsed]);
  aetBuffer = getBuffer + (workBuffer[GWGETUsed]);

  /* Make sure we don't exceed the work buffer */
  if ((((GWHeaderSize + (workBuffer[GWObjUsed])) + (workBuffer[GWGETUsed])) +
       (workBuffer[GWAETUsed])) > (workBuffer[GWSize])) {
    return GEFWorkTooBig;
  }
  return 0;
}