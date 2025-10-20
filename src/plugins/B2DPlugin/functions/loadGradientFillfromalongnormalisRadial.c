/* Extracted from B2DPlugin.c:4851 (function loadGradientFillfromalongnormalisRadial). */

static sqInt loadGradientFillfromalongnormalisRadial(sqInt rampOop, int *point1,
                                                     int *point2, int *point3,
                                                     sqInt isRadial) {
  sqInt fill;
  sqInt fillSize;
  sqInt fillSqInt;
  sqInt i;
  int *rampPtr;
  sqInt rampWidth;

  if (!((fetchClassOf(rampOop)) == (classBitmap()))) {
    return primitiveFail();
  }
  rampWidth = slotSizeOf(rampOop);

  /* begin allocateGradientFill:rampWidth:isRadial: */
  fillSize = GGBaseSize + rampWidth;
  if (!(allocateObjEntry(fillSize))) {
    fill = 0;
  } else {
    fillSqInt = objUsed;
    objUsed = fillSqInt + fillSize;
    if (isRadial) {
      /* #objectTypeOf:put: #obj:at:put: */
      objBuffer[fillSqInt + GEObjectType] = GEPrimitiveRadialGradientFill;
    } else {
      /* #objectTypeOf:put: #obj:at:put: */
      objBuffer[fillSqInt + GEObjectType] = GEPrimitiveLinearGradientFill;
    }

    /* #objectIndexOf:put: #obj:at:put: */
    objBuffer[fillSqInt + GEObjectIndex] = 0;

    /* #objectLengthOf:put: #obj:at:put: */
    objBuffer[fillSqInt + GEObjectLength] = fillSize;
    rampPtr = (objBuffer + fillSqInt) + GFRampOffset;
    if (workBuffer[GWHasColorTransform]) {
      for (i = 0; i < rampWidth; i += 1) {
        rampPtr[i] =
            (transformColor((((int *)(firstIndexableField(rampOop))))[i]));
      }
    } else {
      for (i = 0; i < rampWidth; i += 1) {
        rampPtr[i] = ((((int *)(firstIndexableField(rampOop))))[i]);
      }
    }

    /* #gradientRampLengthOf:put: #obj:at:put: */
    objBuffer[fillSqInt + GFRampLength] = rampWidth;
    fill = fillSqInt;
  }
  /* end allocateGradientFill:rampWidth:isRadial: */
  if (engineStopped) {
    return null;
  }
  loadFillOrientationfromalongnormalwidthheight(fill, point1, point2, point3,
                                                rampWidth, rampWidth);
  return fill;
}