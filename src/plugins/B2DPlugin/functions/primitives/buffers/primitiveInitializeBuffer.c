/* Extracted from B2DPlugin.c:8092 (function primitiveInitializeBuffer). */

EXPORT(sqInt)
primitiveInitializeBuffer(void) {
  sqInt size;
  sqInt wbOop;

  wbOop = 0;
  if (!(((methodArgumentCount()) == 1) &&
        ((isWords((wbOop = stackValue(0)))) &&
         (((size = slotSizeOf(wbOop))) >= GWMinimalSize)))) {
    return primitiveFail();
  }

  /* begin workBufferPut: */
  workBuffer = firstIndexableField(wbOop);
  objBuffer = workBuffer + GWHeaderSize;

  /* magicNumberPut: */
  workBuffer[GWMagicIndex] = GWMagicNumber;

  /* wbSizePut: */
  workBuffer[GWSize] = size;

  /* wbTopPut: */
  workBuffer[GWBufferTop] = size;

  /* statePut: */
  workBuffer[GWState] = GEStateUnlocked;

  /* objStartPut: */
  workBuffer[GWObjStart] = GWHeaderSize;

  /* objUsedPut: */
  workBuffer[GWObjUsed] = 4;

  /* #objectTypeOf:put: #obj:at:put: */
  objBuffer[0 /* (0 + GEObjectType) */] = GEPrimitiveFill;

  /* #objectLengthOf:put: #obj:at:put: */
  objBuffer[1 /* (0 + GEObjectLength) */] = 4;

  /* #objectIndexOf:put: #obj:at:put: */
  objBuffer[2 /* (0 + GEObjectIndex) */] = 0;

  /* getStartPut: */
  workBuffer[GWGETStart] = 0;

  /* getUsedPut: */
  workBuffer[GWGETUsed] = 0;

  /* aetStartPut: */
  workBuffer[GWAETStart] = 0;

  /* aetUsedPut: */
  workBuffer[GWAETUsed] = 0;

  /* stopReasonPut: */
  workBuffer[GWStopReason] = 0;

  /* needsFlushPut: */
  workBuffer[GWNeedsFlush] = 0;

  /* clipMinXPut: */
  workBuffer[GWClipMinX] = 0;

  /* clipMaxXPut: */
  workBuffer[GWClipMaxX] = 0;

  /* clipMinYPut: */
  workBuffer[GWClipMinY] = 0;

  /* clipMaxYPut: */
  workBuffer[GWClipMaxY] = 0;

  /* currentZPut: */
  workBuffer[GWCurrentZ] = 0;
  resetGraphicsEngineStats();
  initEdgeTransform();
  initColorTransform();
  popthenPush(2, wbOop);
  return 0;
}