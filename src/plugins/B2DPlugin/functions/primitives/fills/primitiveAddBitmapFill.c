/* Extracted from B2DPlugin.c:6471 (function primitiveAddBitmapFill). */

EXPORT(sqInt)
primitiveAddBitmapFill(void) {
  sqInt bmBits;
  sqInt bmBitsSize;
  sqInt bmDepth;
  sqInt bmFill;
  sqInt bmHeight;
  sqInt bmRaster;
  sqInt bmWidth;
  int *cm;
  int *cmBits;
  sqInt cmOop;
  sqInt cmSize;
  sqInt dirOop;
  sqInt failureCode;
  sqInt fill;
  sqInt fillSize;
  sqInt fillSqInt;
  sqInt formOop;
  sqInt i;
  sqInt nrmOop;
  sqInt originOop;
  sqInt ppw;
  sqInt tileFlag;
  sqInt tileFlagSqInt;
  sqInt xIndex;

  if (!((methodArgumentCount()) == 7)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  xIndex = stackIntegerValue(0);
  if (xIndex <= 0) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  nrmOop = stackObjectValue(1);
  dirOop = stackObjectValue(2);
  originOop = stackObjectValue(3);
  tileFlag = booleanValueOf(stackValue(4));
  cmOop = stackObjectValue(5);
  formOop = stackObjectValue(6);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(7), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }
  loadPointfrom(((int *)(workBuffer + GWPoint1)), originOop);
  loadPointfrom(((int *)(workBuffer + GWPoint2)), dirOop);
  loadPointfrom(((int *)(workBuffer + GWPoint3)), nrmOop);
  if (failed()) {
    return primitiveFailFor(GEFBadPoint);
  }
  tileFlagSqInt = (tileFlag ? 1 : 0);

  /* begin loadBitmapFill:colormap:tile:from:along:normal:xIndex: */
  if (cmOop == (nilObject())) {
    cmSize = 0;
    cmBits = null;
  } else {
    if (!((fetchClassOf(cmOop)) == (classBitmap()))) {
      primitiveFail();
      return primitiveFailFor(GEFEntityLoadFailed);
    }
    cmSize = slotSizeOf(cmOop);
    cmBits = firstIndexableField(cmOop);
  }
  if (!(isPointers(formOop))) {
    primitiveFail();
    return primitiveFailFor(GEFEntityLoadFailed);
  }
  if ((slotSizeOf(formOop)) < 5) {
    primitiveFail();
    return primitiveFailFor(GEFEntityLoadFailed);
  }
  bmBits = fetchPointerofObject(0, formOop);
  if (!((fetchClassOf(bmBits)) == (classBitmap()))) {
    primitiveFail();
    return primitiveFailFor(GEFEntityLoadFailed);
  }
  bmBitsSize = slotSizeOf(bmBits);
  bmWidth = fetchIntegerofObject(1, formOop);
  bmHeight = fetchIntegerofObject(2, formOop);
  bmDepth = fetchIntegerofObject(3, formOop);
  if (failed()) {
    return primitiveFailFor(GEFEntityLoadFailed);
  }
  if (!((bmWidth >= 0) && (bmHeight >= 0))) {
    primitiveFail();
    return primitiveFailFor(GEFEntityLoadFailed);
  }
  if (!((((((bmDepth == 32) || (bmDepth == 8)) || (bmDepth == 16)) ||
          (bmDepth == 1)) ||
         (bmDepth == 2)) ||
        (bmDepth == 4))) {
    primitiveFail();
    return primitiveFailFor(GEFEntityLoadFailed);
  }
  if (!((cmSize == 0) || (cmSize == (1U << bmDepth)))) {
    primitiveFail();
    return primitiveFailFor(GEFEntityLoadFailed);
  }
  ppw = 32 / bmDepth;
  bmRaster = (bmWidth + (ppw - 1)) / ppw;
  if (!(bmBitsSize == (bmRaster * bmHeight))) {
    primitiveFail();
    return primitiveFailFor(GEFEntityLoadFailed);
  }

  /* begin allocateBitmapFill:colormap: */
  fillSize = GBMBaseSize + cmSize;
  if (!(allocateObjEntry(fillSize))) {
    return primitiveFailFor(GEFEngineStopped);
  }
  fillSqInt = objUsed;
  objUsed = fillSqInt + fillSize;

  /* #objectTypeOf:put: #obj:at:put: */
  objBuffer[fillSqInt + GEObjectType] = GEPrimitiveClippedBitmapFill;

  /* #objectIndexOf:put: #obj:at:put: */
  objBuffer[fillSqInt + GEObjectIndex] = 0;

  /* #objectLengthOf:put: #obj:at:put: */
  objBuffer[fillSqInt + GEObjectLength] = fillSize;
  cm = (objBuffer + fillSqInt) + GBColormapOffset;
  if (workBuffer[GWHasColorTransform]) {
    for (i = 0; i < cmSize; i += 1) {
      cm[i] = (transformColor(cmBits[i]));
    }
  } else {
    for (i = 0; i < cmSize; i += 1) {
      cm[i] = (cmBits[i]);
    }
  }

  /* #bitmapCmSizeOf:put: #obj:at:put: */
  objBuffer[fillSqInt + GBColormapSize] = cmSize;
  bmFill = fillSqInt;
  /* end allocateBitmapFill:colormap: */

  /* #bitmapWidthOf:put: #obj:at:put: */
  objBuffer[bmFill + GBBitmapWidth] = bmWidth;

  /* #bitmapHeightOf:put: #obj:at:put: */
  objBuffer[bmFill + GBBitmapHeight] = bmHeight;

  /* #bitmapDepthOf:put: #obj:at:put: */
  objBuffer[bmFill + GBBitmapDepth] = bmDepth;

  /* #bitmapRasterOf:put: #obj:at:put: */
  objBuffer[bmFill + GBBitmapRaster] = bmRaster;

  /* #bitmapSizeOf:put: #obj:at:put: */
  objBuffer[bmFill + GBBitmapSize] = bmBitsSize;

  /* #bitmapTileFlagOf:put: #obj:at:put: */
  objBuffer[bmFill + GBTileFlag] = tileFlagSqInt;

  /* #objectIndexOf:put: #obj:at:put: */
  objBuffer[bmFill + GEObjectIndex] = (xIndex - 1);
  loadFillOrientationfromalongnormalwidthheight(
      bmFill, ((int *)(workBuffer + GWPoint1)),
      ((int *)(workBuffer + GWPoint2)), ((int *)(workBuffer + GWPoint3)),
      bmWidth, bmHeight);
  fill = bmFill;
  /* end loadBitmapFill:colormap:tile:from:along:normal:xIndex: */
  if (engineStopped) {
    return primitiveFailFor(GEFEngineStopped);
  }

  /* Make sure the stack is okay */
  if (failed()) {
    return primitiveFailFor(GEFEntityLoadFailed);
  }

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  popthenPush(8, positive32BitIntegerFor(fill));
  return 0;
}