/* Extracted from B2DPlugin.c:10755 (function toggleWideFillOf). */

static sqInt toggleWideFillOf(sqInt edge) {
  sqInt depth;
  sqInt fill;
  sqInt index;
  sqInt lineWidth;
  sqInt rightX;
  sqInt type;

  type =
      ((usqInt)(((objBuffer[edge + GEObjectType]) & GEPrimitiveTypeMask))) >> 1;
  dispatchedValue = edge;
  switch (type) {
  case 0: // errorWrongIndex
  case 1: // errorWrongIndex
    error("BalloonEngine: Fatal dispatch error");
    break;
  case 2: // returnWideLineWidth
    dispatchReturnValue = objBuffer[dispatchedValue + GLWideWidth];
    break;
  case 3: // returnWideBezierWidth
    dispatchReturnValue = objBuffer[dispatchedValue + GBWideWidth];
    break;
  }
  lineWidth = dispatchReturnValue;
  switch (type) {
  case 0: // errorWrongIndex
  case 1: // errorWrongIndex
    error("BalloonEngine: Fatal dispatch error");
    break;
  case 2: // returnWideLineFill
    dispatchReturnValue = objBuffer[dispatchedValue + GLWideFill];
    break;
  case 3: // returnWideBezierFill
    dispatchReturnValue = objBuffer[dispatchedValue + GBWideFill];
    break;
  }
  fill = dispatchReturnValue;
  if (!fill) {
    return null;
  }

  /* begin needAvailableSpace: */
  if (((((GWHeaderSize + objUsed) + (workBuffer[GWGETUsed])) +
        (workBuffer[GWAETUsed])) +
       3 /* stackFillEntryLength */) > (workBuffer[GWBufferTop])) {
    /* begin stopBecauseOf: */
    /* stopReasonPut: */
    workBuffer[GWStopReason] = GErrorNoMoreSpace;
    engineStopped = 1;
    return null;
  }

  /* So lines sort before interior fills */
  depth = ((((usqInt)((objBuffer[edge + GEZValue])) << 1))) + 1;
  rightX = (objBuffer[edge + GEXValue]) + lineWidth;
  index = findStackFilldepth(fill, depth);
  if (index == -1) {
    showFilldepthrightX(fill, depth, rightX);
  } else {
    if ((workBuffer[(workBuffer[GWBufferTop]) + (index + 2)]) < rightX) {
      /* #stackFillRightX:put: #wbStackValue:put: */
      workBuffer[(workBuffer[GWBufferTop]) + (index + 2)] = rightX;
    }
  }

  /* begin quickRemoveInvalidFillsAt: */
  if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
    while ((topRightX()) <= (objBuffer[edge + GEXValue])) {
      hideFilldepth(topFill(), topDepth());
      if (!((workBuffer[GWSize]) - (workBuffer[GWBufferTop]))) {
        break;
      }
    }
  }
  /* end quickRemoveInvalidFillsAt: */
  return 0;
}