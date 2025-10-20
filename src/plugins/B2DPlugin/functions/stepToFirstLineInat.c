/* Extracted from B2DPlugin.c:9929 (function stepToFirstLineInat). */

static sqInt stepToFirstLineInat(sqInt line, sqInt yValue) {
  int deltaX;
  int deltaY;
  int err;
  int error;
  int errorAdjUp;
  sqInt i;
  int startY;
  int widthX;
  int x;
  sqInt xDir;
  int xInc;

  /* Do a quick check if there is anything at all to do */
  if ((!((((objBuffer[line + GEObjectType]) & GEPrimitiveTypeMask) &
          GEPrimitiveWide) != 0)) &&
      (yValue >= (objBuffer[line + GLEndY]))) {
    return objBuffer[line + GENumLines] = 0;
  }
  deltaX = (objBuffer[line + GLEndX]) - (objBuffer[line + GEXValue]);
  deltaY = (objBuffer[line + GLEndY]) - (objBuffer[line + GEYValue]);

  /* Check if edge goes left to right */
  if (deltaX >= 0) {
    xDir = 1;
    widthX = deltaX;
    error = 0;
  } else {
    xDir = -1;
    widthX = 0 - deltaX;
    error = 1 - deltaY;
  }

  /* Check if deltaY is zero.
     Note: We could actually get out here immediately
     but wide lines rely on an accurate setup in this case */
  if (deltaY) {
    if (deltaY > widthX) {
      xInc = 0;
      errorAdjUp = widthX;
    } else {
      xInc = (widthX / deltaY) * xDir;
      errorAdjUp = widthX % deltaY;
    }
  } else {
    /* No error for horizontal edges */
    error = 0;

    /* Encodes width and direction */
    xInc = deltaX;
    errorAdjUp = 0;
  }

  /* Check if edge is y-major
     Store the values */

  /* #edgeNumLinesOf:put: #obj:at:put: */
  objBuffer[line + GENumLines] = deltaY;

  /* #lineXDirectionOf:put: #obj:at:put: */
  objBuffer[line + GLXDirection] = xDir;

  /* self lineYDirectionOf: line put: yDir.
     <-- Already set */

  /* #lineXIncrementOf:put: #obj:at:put: */
  objBuffer[line + GLXIncrement] = xInc;

  /* #lineErrorOf:put: #obj:at:put: */
  objBuffer[line + GLError] = error;

  /* #lineErrorAdjUpOf:put: #obj:at:put: */
  objBuffer[line + GLErrorAdjUp] = errorAdjUp;

  /* #lineErrorAdjDownOf:put: #obj:at:put: */
  objBuffer[line + GLErrorAdjDown] = deltaY;

  /* And step to the first scan line */
  if (!(((startY = objBuffer[line + GEYValue])) == yValue)) {
    for (i = startY; i < yValue; i += 1) {
      /* begin stepToNextLineIn:at: */
      x = (objBuffer[line + GEXValue]) + (objBuffer[line + GLXIncrement]);
      err = (objBuffer[line + GLError]) + (objBuffer[line + GLErrorAdjUp]);
      if (err > 0) {
        x += objBuffer[line + GLXDirection];
        err -= objBuffer[line + GLErrorAdjDown];
      }

      /* #lineErrorOf:put: #obj:at:put: */
      objBuffer[line + GLError] = err;

      /* #edgeXValueOf:put: #obj:at:put: */
      objBuffer[line + GEXValue] = x;
    }

    /* Adjust number of lines remaining */

    /* #edgeNumLinesOf:put: #obj:at:put: */
    objBuffer[line + GENumLines] = (deltaY - (yValue - startY));
  }
  return 0;
}