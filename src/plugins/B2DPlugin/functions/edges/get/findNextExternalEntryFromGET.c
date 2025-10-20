/* Extracted from B2DPlugin.c:2961 (function findNextExternalEntryFromGET). */

static sqInt findNextExternalEntryFromGET(void) {
  int edge;
  unsigned int type;
  int yValue;

  yValue = workBuffer[GWCurrentY];

  /* As long as we have entries in the GET */
  while ((workBuffer[GWGETStart]) < (workBuffer[GWGETUsed])) {
    edge = getBuffer[workBuffer[GWGETStart]];
    if ((objBuffer[edge + GEYValue]) > yValue) {
      return 0;
    }
    type = (objBuffer[edge + GEObjectType]) & GEPrimitiveTypeMask;
    if ((type & GEPrimitiveWideMask) == GEPrimitiveEdge) {
      return 1;
    }

    /* begin needAvailableSpace: */
    if (((((GWHeaderSize + objUsed) + (workBuffer[GWGETUsed])) +
          (workBuffer[GWAETUsed])) +
         1) > (workBuffer[GWBufferTop])) {
      /* begin stopBecauseOf: */
      /* stopReasonPut: */
      workBuffer[GWStopReason] = GErrorNoMoreSpace;
      engineStopped = 1;
      return 0;
    }
    switch (type) {
    case 0: // errorWrongIndex
    case 1: // errorWrongIndex
    case 2: // errorWrongIndex
    case 3: // errorWrongIndex
      error("BalloonEngine: Fatal dispatch error");
      break;
    case 4: // stepToFirstLine
      stepToFirstLineInat(getBuffer[workBuffer[GWGETStart]],
                          workBuffer[GWCurrentY]);
      break;
    case 5: // stepToFirstWideLine
      stepToFirstWideLineInat(getBuffer[workBuffer[GWGETStart]],
                              workBuffer[GWCurrentY]);
      break;
    case 6: // stepToFirstBezier
      stepToFirstBezierInat(getBuffer[workBuffer[GWGETStart]],
                            workBuffer[GWCurrentY]);
      break;
    case 7: // stepToFirstWideBezier
      stepToFirstWideBezierInat(getBuffer[workBuffer[GWGETStart]],
                                workBuffer[GWCurrentY]);
      break;
    }

    /* Insert the edge into the AET */
    insertEdgeIntoAET(edge);

    /* getStartPut: */
    workBuffer[GWGETStart] = ((workBuffer[GWGETStart]) + 1);
  }

  /* No entries in GET */
  return 0;
}