/* Extracted from B2DPlugin.c:5454 (function loadWideLinefromtolineFillleftFillrightFill). */

static sqInt loadWideLinefromtolineFillleftFillrightFill(sqInt lineWidth,
                                                         int *p1, int *p2,
                                                         sqInt lineFill,
                                                         sqInt leftFill,
                                                         sqInt rightFill) {
  sqInt line;
  sqInt lineSqInt;
  sqInt offset;
  int *p11;
  int *p21;
  sqInt yDir;

  if ((lineWidth == 0) || (lineFill == 0)) {
    /* begin allocateLine */
    if (!(allocateObjEntry(GLBaseSize))) {
      line = 0;
    } else {
      lineSqInt = objUsed;
      objUsed = lineSqInt + GLBaseSize;

      /* #objectTypeOf:put: #obj:at:put: */
      objBuffer[lineSqInt + GEObjectType] = GEPrimitiveLine;

      /* #objectIndexOf:put: #obj:at:put: */
      objBuffer[lineSqInt + GEObjectIndex] = 0;

      /* #objectLengthOf:put: #obj:at:put: */
      objBuffer[lineSqInt + GEObjectLength] = GLBaseSize;
      line = lineSqInt;
    }
    /* end allocateLine */
    offset = 0;
  } else {
    /* begin allocateWideLine */
    if (!(allocateObjEntry(GLWideSize))) {
      line = 0;
    } else {
      lineSqInt = objUsed;
      objUsed = lineSqInt + GLWideSize;

      /* #objectTypeOf:put: #obj:at:put: */
      objBuffer[lineSqInt + GEObjectType] = GEPrimitiveWideLine;

      /* #objectIndexOf:put: #obj:at:put: */
      objBuffer[lineSqInt + GEObjectIndex] = 0;

      /* #objectLengthOf:put: #obj:at:put: */
      objBuffer[lineSqInt + GEObjectLength] = GLWideSize;
      line = lineSqInt;
    }
    /* end allocateWideLine */
    offset = lineWidth / 2;
  }
  if (engineStopped) {
    return 0;
  }

  /* begin loadLine:from:to:offset:leftFill:rightFill: */
  if ((p1[1]) <= (p2[1])) {
    p11 = p1;
    p21 = p2;
    yDir = 1;
  } else {
    p11 = p2;
    p21 = p1;
    yDir = -1;
  }

  /* #edgeXValueOf:put: #obj:at:put: */
  objBuffer[line + GEXValue] = (p11[0]);

  /* #edgeYValueOf:put: #obj:at:put: */
  objBuffer[line + GEYValue] = ((p11[1]) - offset);

  /* #edgeZValueOf:put: #obj:at:put: */
  objBuffer[line + GEZValue] = (workBuffer[GWCurrentZ]);

  /* #edgeLeftFillOf:put: #obj:at:put: */
  objBuffer[line + GEFillIndexLeft] = leftFill;

  /* #edgeRightFillOf:put: #obj:at:put: */
  objBuffer[line + GEFillIndexRight] = rightFill;

  /* #lineEndXOf:put: #obj:at:put: */
  objBuffer[line + GLEndX] = (p21[0]);

  /* #lineEndYOf:put: #obj:at:put: */
  objBuffer[line + GLEndY] = ((p21[1]) - offset);

  /* #lineYDirectionOf:put: #obj:at:put: */
  objBuffer[line + GLYDirection] = yDir;
  if (((objBuffer[line + GEObjectType]) & GEPrimitiveTypeMask) &
      GEPrimitiveWide) {
    /* #wideLineFillOf:put: #obj:at:put: */
    objBuffer[line + GLWideFill] = lineFill;

    /* #wideLineWidthOf:put: #obj:at:put: */
    objBuffer[line + GLWideWidth] = lineWidth;

    /* #wideLineExtentOf:put: #obj:at:put: */
    objBuffer[line + GLWideExtent] = lineWidth;
  }
  return 0;
}