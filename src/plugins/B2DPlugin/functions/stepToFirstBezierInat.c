/* Extracted from B2DPlugin.c:9806 (function stepToFirstBezierInat). */

static sqInt stepToFirstBezierInat(sqInt bezier, sqInt yValue) {
  int deltaY;
  int endX;
  int endY;
  sqInt fwDDx;
  sqInt fwDDy;
  int fwDx;
  int fwDxInt;
  int fwDy;
  int fwDyInt;
  int fwX1;
  int fwX2;
  int fwY1;
  int fwY2;
  int lastX;
  int lastY;
  int maxSteps;
  sqInt minY;
  int scaledStepSize;
  sqInt squaredStepSize;
  int startX;
  int startY;
  int *updateData;
  int *updateData1;
  int viaX;
  int viaY;
  unsigned int word1;
  unsigned int word2;
  sqInt xValue;

  /* Do a quick check if there is anything at all to do */
  if ((!((((objBuffer[bezier + GEObjectType]) & GEPrimitiveTypeMask) &
          GEPrimitiveWide) != 0)) &&
      (yValue >= (objBuffer[bezier + GBEndY]))) {
    return objBuffer[bezier + GENumLines] = 0;
  }

  /* Now really initialize bezier */
  startX = objBuffer[bezier + GEXValue];
  startY = objBuffer[bezier + GEYValue];
  viaX = objBuffer[bezier + GBViaX];
  viaY = objBuffer[bezier + GBViaY];
  endX = objBuffer[bezier + GBEndX];
  endY = objBuffer[bezier + GBEndY];
  deltaY = endY - startY;

  /* Initialize integer forward differencing */
  fwX1 = (viaX - startX) * 2;
  fwX2 = (startX + endX) - (viaX * 2);
  fwY1 = (viaY - startY) * 2;
  fwY2 = (startY + endY) - (viaY * 2);
  maxSteps = deltaY * 2;
  if (maxSteps < 2) {
    maxSteps = 2;
  }
  scaledStepSize = 0x1000000 / maxSteps;

  /* begin absoluteSquared8Dot24: */
  word1 = scaledStepSize & 0xFFFF;
  word2 = ((((usqInt)(scaledStepSize)) >> 16)) & 0xFF;
  squaredStepSize = (((usqInt)(((((((usqInt)((word1 * word1))) >> 16)) +
                                 ((word1 * word2) * 2)) +
                                (((((usqInt)((word2 * word2)) << 16))))))) >>
                     8);
  fwDx = fwX1 * scaledStepSize;
  fwDDx = (fwX2 * squaredStepSize) * 2;
  fwDx += fwDDx / 2;
  fwDy = fwY1 * scaledStepSize;
  fwDDy = (fwY2 * squaredStepSize) * 2;
  fwDy += fwDDy / 2;

  /* Store the values */

  /* #edgeNumLinesOf:put: #obj:at:put: */
  objBuffer[bezier + GENumLines] = deltaY;
  updateData = (objBuffer + bezier) + GBUpdateData;
  updateData[GBUpdateX] = (startX * 0x100);
  updateData[GBUpdateY] = (startY * 0x100);
  updateData[GBUpdateDX] = fwDx;
  updateData[GBUpdateDY] = fwDy;
  updateData[GBUpdateDDX] = fwDDx;
  updateData[GBUpdateDDY] = fwDDy;

  /* And step to the first scan line */
  if (!(((startY = objBuffer[bezier + GEYValue])) == yValue)) {
    /* begin stepToNextBezierIn:at: */
    updateData1 = (objBuffer + bezier) + GBUpdateData;
    lastX = updateData1[GBUpdateX];
    lastY = updateData1[GBUpdateY];
    fwDxInt = updateData1[GBUpdateDX];
    fwDyInt = updateData1[GBUpdateDY];
    minY = yValue * 0x100;

    /* Step as long as we haven't yet reached minY and also
       as long as fwDy is greater than zero thus stepping down.
       Note: The test for fwDy should not be necessary in theory
       but is a good insurance in practice. */
    while ((minY > lastY) && (fwDyInt >= 0)) {
      lastX += (((sqInt)((fwDxInt + 0x8000))) >> 16);
      lastY += (((sqInt)((fwDyInt + 0x8000))) >> 16);
      fwDxInt += updateData1[GBUpdateDDX];
      fwDyInt += updateData1[GBUpdateDDY];
    }
    updateData1[GBUpdateX] = lastX;
    updateData1[GBUpdateY] = lastY;
    updateData1[GBUpdateDX] = fwDxInt;
    updateData1[GBUpdateDY] = fwDyInt;
    xValue = (((sqInt)(lastX)) >> 8);

    /* #edgeXValueOf:put: #obj:at:put: */
    objBuffer[bezier + GEXValue] = xValue;

    /* Adjust number of lines remaining */

    /* #edgeNumLinesOf:put: #obj:at:put: */
    objBuffer[bezier + GENumLines] = (deltaY - (yValue - startY));
  }
  return 0;
}