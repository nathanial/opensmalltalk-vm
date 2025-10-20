/* Extracted from B2DPlugin.c:5105 (function loadRenderingState). */

static sqInt loadRenderingState(void) {
  sqInt edgeOop;
  sqInt failCode;
  sqInt fillOop;
  int state;

  if (!((methodArgumentCount()) == 2)) {
    return PrimErrBadNumArgs;
  }
  if ((failCode = quickLoadEngineFrom(stackValue(2)))) {
    return failCode;
  }
  fillOop = stackObjectValue(0);
  edgeOop = stackObjectValue(1);
  if (failed()) {
    return PrimErrBadArgument;
  }

  /* Load span buffer and bitBlt */
  if ((failCode =
           loadSpanBufferFrom(fetchPointerofObject(BESpanIndex, engine)))) {
    return failCode;
  }
  if (!(loadBitBltFrom(fetchPointerofObject(BEBitBltIndex, engine)))) {
    return GEFBitBltLoadFailed;
  }
  if (!(loadFormsFrom(fetchPointerofObject(BEFormsIndex, engine)))) {
    return GEFFormLoadFailed;
  }

  /* Check edgeOop and fillOop */
  if ((slotSizeOf(edgeOop)) < ETBalloonEdgeDataSize) {
    return GEFEdgeDataTooSmall;
  }
  if ((slotSizeOf(fillOop)) < FTBalloonFillDataSize) {
    return GEFFillDataTooSmall;
  }

  /* Note: Rendering can only take place if we're not in one of the intermediate
     (e.g., external) states. */
  state = workBuffer[GWState];
  if ((state == GEStateWaitingForEdge) ||
      ((state == GEStateWaitingForFill) || (state == GEStateWaitingChange))) {
    return GEFWrongState;
  }
  return 0;
}