/* Extracted from B2DPlugin.c:6924 (function primitiveAddOval). */

EXPORT(sqInt)
primitiveAddOval(void) {
  usqInt borderIndex;
  sqInt borderWidth;
  sqInt endOop;
  sqInt failureCode;
  usqInt fillIndex;
  sqInt startOop;

  if (!((methodArgumentCount()) == 5)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  borderIndex = positive32BitValueOf(stackValue(0));
  borderWidth = stackIntegerValue(1);
  fillIndex = positive32BitValueOf(stackValue(2));
  endOop = stackObjectValue(3);
  startOop = stackObjectValue(4);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(5), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }

  /* Make sure the fills are okay */
  if (!((isFillOkay(borderIndex)) && (isFillOkay(fillIndex)))) {
    return primitiveFailFor(GEFWrongFill);
  }

  /* Transform colors */
  fillIndex = transformColor(fillIndex);
  borderIndex = transformColor(borderIndex);
  if (engineStopped) {
    return primitiveFailFor(GEFEngineStopped);
  }

  /* Check if we have anything at all to do */
  if ((fillIndex == 0) && ((borderIndex == 0) || (borderWidth <= 0))) {
    return pop(5);
  }

  /* Make sure we have some space */

  /* begin needAvailableSpace: */
  if (((((GWHeaderSize + objUsed) + (workBuffer[GWGETUsed])) +
        (workBuffer[GWAETUsed])) +
       (16 * GBBaseSize)) > (workBuffer[GWBufferTop])) {
    /* begin stopBecauseOf: */
    /* stopReasonPut: */
    workBuffer[GWStopReason] = GErrorNoMoreSpace;
    engineStopped = 1;
    return primitiveFailFor(GEFWorkTooBig);
  }

  /* Check if we need a border */
  if ((borderWidth > 0) && (borderIndex != 0)) {
    borderWidth = transformWidth(borderWidth);
  } else {
    borderWidth = 0;
  }

  /* Load the rectangle points */
  loadPointfrom(((int *)(workBuffer + GWPoint1)), startOop);
  loadPointfrom(((int *)(workBuffer + GWPoint2)), endOop);
  if (failed()) {
    return primitiveFailFor(GEFBadPoint);
  }
  loadOvallineFillleftFillrightFill(borderWidth, borderIndex, 0, fillIndex);
  if (engineStopped) {
    /* begin wbStackClear */
    /* wbTopPut: */
    workBuffer[GWBufferTop] = (workBuffer[GWSize]);
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
  pop(5);
  return 0;
}