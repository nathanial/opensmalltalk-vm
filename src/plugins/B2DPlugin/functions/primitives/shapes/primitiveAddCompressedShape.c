/* Extracted from B2DPlugin.c:6657 (function primitiveAddCompressedShape). */

EXPORT(sqInt)
primitiveAddCompressedShape(void) {
  sqInt failureCode;
  sqInt fillIndexList;
  sqInt leftFills;
  sqInt lineFills;
  sqInt lineWidths;
  sqInt nSegments;
  sqInt points;
  sqInt rightFills;

  if (!((methodArgumentCount()) == 7)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  fillIndexList = stackObjectValue(0);
  lineFills = stackObjectValue(1);
  lineWidths = stackObjectValue(2);
  rightFills = stackObjectValue(3);
  leftFills = stackObjectValue(4);
  nSegments = stackIntegerValue(5);
  points = stackObjectValue(6);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(7), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }

  /* First, do a check if the compressed shape is okay */
  if (!(checkCompressedShapesegmentsleftFillsrightFillslineWidthslineFillsfillIndexList(
          points, nSegments, leftFills, rightFills, lineWidths, lineFills,
          fillIndexList))) {
    return primitiveFailFor(GEFEntityCheckFailed);
  }

  /* Now check that we have some hope to have enough free space.
     Do this by assuming nSegments boundaries of maximum size,
     hoping that most of the fills will be colors and many boundaries
     will be line segments */

  /* begin needAvailableSpace: */
  if (((((GWHeaderSize + objUsed) + (workBuffer[GWGETUsed])) +
        (workBuffer[GWAETUsed])) +
       ((((GBBaseSize < GLBaseSize) ? GLBaseSize : GBBaseSize)) * nSegments)) >
      (workBuffer[GWBufferTop])) {
    /* begin stopBecauseOf: */
    /* stopReasonPut: */
    workBuffer[GWStopReason] = GErrorNoMoreSpace;
    engineStopped = 1;
    return primitiveFailFor(GEFWorkTooBig);
  }

  /* Check if the points are short, and load as appropriate. */
  if ((slotSizeOf(points)) == (nSegments * 3)) {
    loadCompressedShapeFromShortPointssegmentsleftFillsrightFillslineWidthslineFillsfillIndexList(
        ((short *)(firstIndexableField(points))), nSegments,
        ((int *)(firstIndexableField(leftFills))),
        ((int *)(firstIndexableField(rightFills))),
        ((int *)(firstIndexableField(lineWidths))),
        ((int *)(firstIndexableField(lineFills))),
        ((int *)(firstIndexableField(fillIndexList))));
  } else {
    loadCompressedShapeFromIntPointssegmentsleftFillsrightFillslineWidthslineFillsfillIndexList(
        ((int *)(firstIndexableField(points))), nSegments,
        ((int *)(firstIndexableField(leftFills))),
        ((int *)(firstIndexableField(rightFills))),
        ((int *)(firstIndexableField(lineWidths))),
        ((int *)(firstIndexableField(lineFills))),
        ((int *)(firstIndexableField(fillIndexList))));
  }
  if (engineStopped) {
    return primitiveFailFor(GEFEngineStopped);
  }
  if (failed()) {
    return primitiveFailFor(GEFEntityLoadFailed);
  }

  /* needsFlushPut: */
  workBuffer[GWNeedsFlush] = 1;

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(7);
  return 0;
}