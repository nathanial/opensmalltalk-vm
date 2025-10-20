/* Extracted from B2DPlugin.c:10138 (function stepToFirstWideLineInat). */

static sqInt stepToFirstWideLineInat(sqInt line, sqInt yValue) {
  int err;
  sqInt i;
  int lastX;
  sqInt lineOffset;
  sqInt lineOffsetSqInt;
  int lineWidth;
  int lineWidthInt;
  int nextX;
  int nLines;
  int startX;
  int startY;
  int x;
  int xDir;
  sqInt yEntry;
  int yEntryInt;
  sqInt yExit;
  int yExitInt;

  /* Get some values */
  lineWidth = objBuffer[line + GLWideExtent];
  lineOffset = lineWidth / 2;

  /* Compute the incremental values of the line */
  startX = objBuffer[line + GEXValue];
  startY = objBuffer[line + GEYValue];
  stepToFirstLineInat(line, startY);
  nLines = objBuffer[line + GENumLines];
  xDir = objBuffer[line + GLXDirection];

  /* Adjust the line to start at the correct X position */

  /* #edgeXValueOf:put: #obj:at:put: */
  objBuffer[line + GEXValue] = (startX - lineOffset);

  /* Adjust the number of lines to include the lineWidth */

  /* #edgeNumLinesOf:put: #obj:at:put: */
  objBuffer[line + GENumLines] = (nLines + lineWidth);

  /* Adjust the values for x-major lines */
  if (xDir > 0) {
    /* #wideLineWidthOf:put: #obj:at:put: */
    objBuffer[line + GLWideWidth] =
        ((objBuffer[line + GLXIncrement]) + lineWidth);
  } else {
    /* #wideLineWidthOf:put: #obj:at:put: */
    objBuffer[line + GLWideWidth] =
        (lineWidth - (objBuffer[line + GLXIncrement]));

    /* #edgeXValueOf:put: #obj:at:put: */
    objBuffer[line + GEXValue] =
        ((objBuffer[line + GEXValue]) + (objBuffer[line + GLXIncrement]));
  }

  /* Compute the points where we have to turn on/off the fills */

  /* turned on at lineOffset */
  yEntry = 0;

  /* turned off at zero */
  yExit = (0 - nLines) - lineOffset;

  /* #wideLineEntryOf:put: #obj:at:put: */
  objBuffer[line + GLWideEntry] = yEntry;

  /* #wideLineExitOf:put: #obj:at:put: */
  objBuffer[line + GLWideExit] = yExit;

  /* Turn the fills on/off as necessary */
  if ((yEntry >= lineOffset) && (yExit < 0)) {
    /* #edgeFillsValidate: #objectTypeOf:put: #obj:at:put: */
    objBuffer[line + GEObjectType] =
        (((objBuffer[line + GEObjectType]) & GEPrimitiveTypeMask) &
         ((unsigned int)~GEEdgeFillsInvalid));
  } else {
    /* #edgeFillsInvalidate: #objectTypeOf:put: #obj:at:put: */
    objBuffer[line + GEObjectType] =
        (((objBuffer[line + GEObjectType]) & GEPrimitiveTypeMask) |
         GEEdgeFillsInvalid);
  }

  /* And step to the first scan line */
  if (!(startY == yValue)) {
    for (i = startY; i < yValue; i += 1) {
      /* begin stepToNextWideLineIn:at: */
      yEntryInt = (objBuffer[line + GLWideEntry]) + 1;
      yExitInt = (objBuffer[line + GLWideExit]) + 1;

      /* #wideLineEntryOf:put: #obj:at:put: */
      objBuffer[line + GLWideEntry] = yEntryInt;

      /* #wideLineExitOf:put: #obj:at:put: */
      objBuffer[line + GLWideExit] = yExitInt;

      /* Turn fills on/off */
      lineWidthInt = objBuffer[line + GLWideExtent];
      lineOffsetSqInt = lineWidthInt / 2;
      if (yEntryInt >= lineOffsetSqInt) {
        /* #edgeFillsValidate: #objectTypeOf:put: #obj:at:put: */
        objBuffer[line + GEObjectType] =
            (((objBuffer[line + GEObjectType]) & GEPrimitiveTypeMask) &
             ((unsigned int)~GEEdgeFillsInvalid));
      }
      if (yExitInt >= 0) {
        /* #edgeFillsInvalidate: #objectTypeOf:put: #obj:at:put: */
        objBuffer[line + GEObjectType] =
            (((objBuffer[line + GEObjectType]) & GEPrimitiveTypeMask) |
             GEEdgeFillsInvalid);
      }

      /* Step to the next scan line */
      lastX = objBuffer[line + GEXValue];

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
      nextX = objBuffer[line + GEXValue];

      /* Check for special start/end adjustments */
      if ((yEntryInt <= lineWidthInt) || ((yExitInt + lineOffsetSqInt) >= 0)) {
        adjustWideLineafterSteppingFromto(line, lastX, nextX);
      }
    }

    /* Adjust number of lines remaining */

    /* #edgeNumLinesOf:put: #obj:at:put: */
    objBuffer[line + GENumLines] =
        ((objBuffer[line + GENumLines]) - (yValue - startY));
  }
  return 0;
}