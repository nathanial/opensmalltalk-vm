/* Extracted from B2DPlugin.c:5368 (function loadWideBezierlineFillleftFillrightFilln). */

static sqInt loadWideBezierlineFillleftFillrightFilln(sqInt lineWidth,
                                                      sqInt lineFill,
                                                      sqInt leftFill,
                                                      sqInt rightFill,
                                                      sqInt nSegments) {
  sqInt bezier;
  sqInt bezierSqInt;
  sqInt index;
  sqInt offset;
  sqInt wide;

  if ((lineWidth == 0) || (lineFill == 0)) {
    wide = 0;
    offset = 0;
  } else {
    wide = 1;
    offset = lineWidth / 2;
  }
  index = nSegments * 6;
  while (index > 0) {
    if (wide) {
      /* begin allocateWideBezier */
      if (!(allocateObjEntry(GBWideSize))) {
        bezier = 0;
      } else {
        bezierSqInt = objUsed;
        objUsed = bezierSqInt + GBWideSize;

        /* #objectTypeOf:put: #obj:at:put: */
        objBuffer[bezierSqInt + GEObjectType] = GEPrimitiveWideBezier;

        /* #objectIndexOf:put: #obj:at:put: */
        objBuffer[bezierSqInt + GEObjectIndex] = 0;

        /* #objectLengthOf:put: #obj:at:put: */
        objBuffer[bezierSqInt + GEObjectLength] = GBWideSize;
        bezier = bezierSqInt;
      }
      /* end allocateWideBezier */
    } else {
      /* begin allocateBezier */
      if (!(allocateObjEntry(GBBaseSize))) {
        bezier = 0;
      } else {
        bezierSqInt = objUsed;
        objUsed = bezierSqInt + GBBaseSize;

        /* #objectTypeOf:put: #obj:at:put: */
        objBuffer[bezierSqInt + GEObjectType] = GEPrimitiveBezier;

        /* #objectIndexOf:put: #obj:at:put: */
        objBuffer[bezierSqInt + GEObjectIndex] = 0;

        /* #objectLengthOf:put: #obj:at:put: */
        objBuffer[bezierSqInt + GEObjectLength] = GBBaseSize;
        bezier = bezierSqInt;
      }
      /* end allocateBezier */
    }
    if (engineStopped) {
      return 0;
    }
    loadBeziersegmentleftFillrightFilloffset(bezier, index, leftFill, rightFill,
                                             offset);
    if (wide) {
      /* #wideBezierFillOf:put: #obj:at:put: */
      objBuffer[bezier + GBWideFill] = lineFill;

      /* #wideBezierWidthOf:put: #obj:at:put: */
      objBuffer[bezier + GBWideWidth] = lineWidth;

      /* #wideBezierExtentOf:put: #obj:at:put: */
      objBuffer[bezier + GBWideExtent] = lineWidth;
    }
    index -= 6;
  }

  /* begin wbStackClear */
  /* wbTopPut: */
  workBuffer[GWBufferTop] = (workBuffer[GWSize]);
  return 0;
}