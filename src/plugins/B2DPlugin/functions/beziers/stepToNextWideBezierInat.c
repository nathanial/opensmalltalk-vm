/* Extracted from B2DPlugin.c:10285 (function stepToNextWideBezierInat). */

static sqInt stepToNextWideBezierInat(sqInt bezier, sqInt yValue) {
  int fwDx;
  int fwDy;
  int lastX;
  int lastY;
  sqInt lineOffset;
  int lineWidth;
  sqInt minY;
  int *updateData;
  int yEntry;
  int yExit;

  /* Don't inline this */
  lineWidth = objBuffer[bezier + GBWideExtent];
  lineOffset = lineWidth / 2;
  yEntry = (objBuffer[bezier + GBWideEntry]) + 1;
  yExit = (objBuffer[bezier + GBWideExit]) + 1;

  /* #wideBezierEntryOf:put: #obj:at:put: */
  objBuffer[bezier + GBWideEntry] = yEntry;

  /* #wideBezierExitOf:put: #obj:at:put: */
  objBuffer[bezier + GBWideExit] = yExit;
  if (yEntry >= lineOffset) {
    /* #edgeFillsValidate: #objectTypeOf:put: #obj:at:put: */
    objBuffer[bezier + GEObjectType] =
        (((objBuffer[bezier + GEObjectType]) & GEPrimitiveTypeMask) &
         ((unsigned int)~GEEdgeFillsInvalid));
  }
  if (yExit >= 0) {
    /* #edgeFillsInvalidate: #objectTypeOf:put: #obj:at:put: */
    objBuffer[bezier + GEObjectType] =
        (((objBuffer[bezier + GEObjectType]) & GEPrimitiveTypeMask) |
         GEEdgeFillsInvalid);
  }

  /* Check if we have to step the upper curve */
  if ((yExit + lineOffset) < 0) {
    updateData = (objBuffer + bezier) + GBUpdateData;

    /* begin stepToNextBezierForward:at: */
    lastX = updateData[GBUpdateX];
    lastY = updateData[GBUpdateY];
    fwDx = updateData[GBUpdateDX];
    fwDy = updateData[GBUpdateDY];
    minY = yValue * 0x100;

    /* Step as long as we haven't yet reached minY and also
       as long as fwDy is greater than zero thus stepping down.
       Note: The test for fwDy should not be necessary in theory
       but is a good insurance in practice. */
    while ((minY > lastY) && (fwDy >= 0)) {
      lastX += (((sqInt)((fwDx + 0x8000))) >> 16);
      lastY += (((sqInt)((fwDy + 0x8000))) >> 16);
      fwDx += updateData[GBUpdateDDX];
      fwDy += updateData[GBUpdateDDY];
    }
    updateData[GBUpdateX] = lastX;
    updateData[GBUpdateY] = lastY;
    updateData[GBUpdateDX] = fwDx;
    updateData[GBUpdateDY] = fwDy;
    (((sqInt)(lastX)) >> 8);
  } else {
    ((objBuffer + bezier) + GBUpdateData)[GBUpdateX] =
        ((objBuffer[bezier + GBFinalX]) * 0x100);
  }

  /* Adjust the last x value to the final x recorded previously
     Step the lower curve */
  updateData = (objBuffer + bezier) + GBWideUpdateData;

  /* begin stepToNextBezierForward:at: */
  lastX = updateData[GBUpdateX];
  lastY = updateData[GBUpdateY];
  fwDx = updateData[GBUpdateDX];
  fwDy = updateData[GBUpdateDY];
  minY = yValue * 0x100;

  /* Step as long as we haven't yet reached minY and also
     as long as fwDy is greater than zero thus stepping down.
     Note: The test for fwDy should not be necessary in theory
     but is a good insurance in practice. */
  while ((minY > lastY) && (fwDy >= 0)) {
    lastX += (((sqInt)((fwDx + 0x8000))) >> 16);
    lastY += (((sqInt)((fwDy + 0x8000))) >> 16);
    fwDx += updateData[GBUpdateDDX];
    fwDy += updateData[GBUpdateDDY];
  }
  updateData[GBUpdateX] = lastX;
  updateData[GBUpdateY] = lastY;
  updateData[GBUpdateDX] = fwDx;
  updateData[GBUpdateDY] = fwDy;
  (((sqInt)(lastX)) >> 8);
  computeFinalWideBezierValueswidth(bezier, lineWidth);
  return 0;
}