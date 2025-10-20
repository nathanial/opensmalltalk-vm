/* Extracted from B2DPlugin.c:3260 (function findNextExternalUpdateFromAET). */

static sqInt findNextExternalUpdateFromAET(void) {
  int count;
  int edge;
  int err;
  int errInt;
  int fwDx;
  int fwDy;
  int lastX;
  int lastXInt;
  int lastY;
  sqInt line;
  sqInt lineOffset;
  sqInt lineSqInt;
  int lineWidth;
  sqInt minY;
  int nextX;
  unsigned int type;
  int *updateData;
  int x;
  int xInt;
  sqInt xValue;
  int yEntry;
  int yExit;

  while ((workBuffer[GWAETStart]) < (workBuffer[GWAETUsed])) {
    edge = aetBuffer[workBuffer[GWAETStart]];
    count = (objBuffer[edge + GENumLines]) - 1;
    if (count) {
      /* #edgeNumLinesOf:put: #obj:at:put: */
      objBuffer[edge + GENumLines] = count;
      type = (objBuffer[edge + GEObjectType]) & GEPrimitiveTypeMask;
      if ((type & GEPrimitiveWideMask) == GEPrimitiveEdge) {
        return 1;
      }
      switch (type) {
      case 0: // errorWrongIndex
      case 1: // errorWrongIndex
      case 2: // errorWrongIndex
      case 3: // errorWrongIndex
        error("BalloonEngine: Fatal dispatch error");
        break;
      case 4: // stepToNextLine
        line = aetBuffer[workBuffer[GWAETStart]];

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
        break;
      case 5: // stepToNextWideLine
        lineSqInt = aetBuffer[workBuffer[GWAETStart]];

        /* begin stepToNextWideLineIn:at: */
        yEntry = (objBuffer[lineSqInt + GLWideEntry]) + 1;
        yExit = (objBuffer[lineSqInt + GLWideExit]) + 1;

        /* #wideLineEntryOf:put: #obj:at:put: */
        objBuffer[lineSqInt + GLWideEntry] = yEntry;

        /* #wideLineExitOf:put: #obj:at:put: */
        objBuffer[lineSqInt + GLWideExit] = yExit;

        /* Turn fills on/off */
        lineWidth = objBuffer[lineSqInt + GLWideExtent];
        lineOffset = lineWidth / 2;
        if (yEntry >= lineOffset) {
          /* #edgeFillsValidate: #objectTypeOf:put: #obj:at:put: */
          objBuffer[lineSqInt + GEObjectType] =
              (((objBuffer[lineSqInt + GEObjectType]) & GEPrimitiveTypeMask) &
               ((unsigned int)~GEEdgeFillsInvalid));
        }
        if (yExit >= 0) {
          /* #edgeFillsInvalidate: #objectTypeOf:put: #obj:at:put: */
          objBuffer[lineSqInt + GEObjectType] =
              (((objBuffer[lineSqInt + GEObjectType]) & GEPrimitiveTypeMask) |
               GEEdgeFillsInvalid);
        }

        /* Step to the next scan line */
        lastXInt = objBuffer[lineSqInt + GEXValue];

        /* begin stepToNextLineIn:at: */
        xInt = (objBuffer[lineSqInt + GEXValue]) +
               (objBuffer[lineSqInt + GLXIncrement]);
        errInt = (objBuffer[lineSqInt + GLError]) +
                 (objBuffer[lineSqInt + GLErrorAdjUp]);
        if (errInt > 0) {
          xInt += objBuffer[lineSqInt + GLXDirection];
          errInt -= objBuffer[lineSqInt + GLErrorAdjDown];
        }

        /* #lineErrorOf:put: #obj:at:put: */
        objBuffer[lineSqInt + GLError] = errInt;

        /* #edgeXValueOf:put: #obj:at:put: */
        objBuffer[lineSqInt + GEXValue] = xInt;
        nextX = objBuffer[lineSqInt + GEXValue];

        /* Check for special start/end adjustments */
        if ((yEntry <= lineWidth) || ((yExit + lineOffset) >= 0)) {
          adjustWideLineafterSteppingFromto(lineSqInt, lastXInt, nextX);
        }
        break;
      case 6: // stepToNextBezier
        updateData =
            (objBuffer + (aetBuffer[workBuffer[GWAETStart]])) + GBUpdateData;

        /* begin stepToNextBezierForward:at: */
        lastX = updateData[GBUpdateX];
        lastY = updateData[GBUpdateY];
        fwDx = updateData[GBUpdateDX];
        fwDy = updateData[GBUpdateDY];
        minY = (workBuffer[GWCurrentY]) * 0x100;

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
        xValue = (((sqInt)(lastX)) >> 8);

        /* #edgeXValueOf:put: #obj:at:put: */
        objBuffer[(aetBuffer[workBuffer[GWAETStart]]) + GEXValue] = xValue;
        break;
      case 7: // stepToNextWideBezier
        stepToNextWideBezierInat(aetBuffer[workBuffer[GWAETStart]],
                                 workBuffer[GWCurrentY]);
        break;
      }
      resortFirstAETEntry();

      /* aetStartPut: */
      workBuffer[GWAETStart] = ((workBuffer[GWAETStart]) + 1);
    } else {
      removeFirstAETEntry();
    }
  }
  return 0;
}