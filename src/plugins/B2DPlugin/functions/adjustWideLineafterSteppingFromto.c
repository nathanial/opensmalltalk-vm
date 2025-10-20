/* Extracted from B2DPlugin.c:701 (function adjustWideLineafterSteppingFromto). */

static sqInt adjustWideLineafterSteppingFromto(sqInt line, sqInt lastX,
                                               sqInt nextX) {
  int baseWidth;
  sqInt deltaX;
  sqInt lineOffset;
  int lineWidth;
  int xDir;
  int yEntry;
  int yExit;

  /* Don't inline this */
  /* Fetch the values the adjustment decisions are based on */
  yEntry = objBuffer[line + GLWideEntry];
  yExit = objBuffer[line + GLWideExit];
  baseWidth = objBuffer[line + GLWideExtent];
  lineOffset = baseWidth / 2;
  lineWidth = objBuffer[line + GLWideWidth];
  xDir = objBuffer[line + GLXDirection];
  deltaX = nextX - lastX;

  /* Adjust the start of the line to fill an entire rectangle */
  if (yEntry < baseWidth) {
    if (xDir < 0) {
      /* effectively adding */
      lineWidth -= deltaX;
    } else {
      lineWidth += deltaX;

      /* #edgeXValueOf:put: #obj:at:put: */
      objBuffer[line + GEXValue] = lastX;
    }
  }

  /* Adjust the end of x-major lines */
  if (!(yExit + lineOffset)) {
    if (xDir > 0) {
      lineWidth -= objBuffer[line + GLXIncrement];
    } else {
      /* effectively subtracting */
      lineWidth += objBuffer[line + GLXIncrement];

      /* #edgeXValueOf:put: #obj:at:put: */
      objBuffer[line + GEXValue] = lastX;
    }
  }

  /* Adjust the end of the line to fill an entire rectangle */
  if ((yExit + lineOffset) > 0) {
    if (xDir < 0) {
      /* effectively subtracting */
      lineWidth += deltaX;

      /* #edgeXValueOf:put: #obj:at:put: */
      objBuffer[line + GEXValue] = lastX;
    } else {
      lineWidth -= deltaX;
    }
  }

  /* Store the manipulated line width back */

  /* #wideLineWidthOf:put: #obj:at:put: */
  objBuffer[line + GLWideWidth] = lineWidth;
  return 0;
}