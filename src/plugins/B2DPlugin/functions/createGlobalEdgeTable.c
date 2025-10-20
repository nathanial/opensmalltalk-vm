/* Extracted from B2DPlugin.c:1415 (function createGlobalEdgeTable). */

static sqInt createGlobalEdgeTable(void) {
  sqInt end;
  int lineWidth;
  sqInt object;

  object = 0;
  end = objUsed;
  while (object < end) {
    if (isEdge(object)) {
      if (!((objBuffer[object + GEYValue]) >= (workBuffer[GWFillMaxY]))) {
        /* begin checkedAddEdgeToGET: */
        if ((((objBuffer[object + GEObjectType]) & GEPrimitiveTypeMask) &
             GEPrimitiveWideMask) == GEPrimitiveLine) {
          /* begin checkedAddLineToGET: */
          if (((objBuffer[object + GEObjectType]) & GEPrimitiveTypeMask) &
              GEPrimitiveWide) {
            lineWidth = objBuffer[object + GLWideExtent];
          } else {
            lineWidth = 0;
          }
          if (!(((objBuffer[object + GLEndY]) + lineWidth) <
              (workBuffer[GWFillMinY]))) {
            /* Overlaps in Y but may still be entirely right of clip region */
            if (!((((objBuffer[object + GEXValue]) - lineWidth) >=
                 (workBuffer[GWFillMaxX])) &&
                (((objBuffer[object + GLEndX]) - lineWidth) >=
                 (workBuffer[GWFillMaxX])))) {
              addEdgeToGET(object);
            }
          }
        } else if ((((objBuffer[object + GEObjectType]) & GEPrimitiveTypeMask) &
             GEPrimitiveWideMask) == GEPrimitiveBezier) {
          /* begin checkedAddBezierToGET: */
          if (((objBuffer[object + GEObjectType]) & GEPrimitiveTypeMask) &
              GEPrimitiveWide) {
            lineWidth = objBuffer[object + GBWideExtent];
          } else {
            lineWidth = 0;
          }
          if (!(((objBuffer[object + GBEndY]) + lineWidth) <
              (workBuffer[GWFillMinY]))) {
            /* Overlaps in Y but may still be entirely right of clip region */
            if (!((((objBuffer[object + GEXValue]) - lineWidth) >=
                 (workBuffer[GWFillMaxX])) &&
                (((objBuffer[object + GBEndX]) - lineWidth) >=
                 (workBuffer[GWFillMaxX])))) {
              addEdgeToGET(object);
            }
          }
        } else {
          addEdgeToGET(object);
        }
        /* end checkedAddEdgeToGET: */
      }
    }

    /* Check if the edge starts below fillMaxY. */
    object += objBuffer[object + GEObjectLength];
  }
  return 0;
}