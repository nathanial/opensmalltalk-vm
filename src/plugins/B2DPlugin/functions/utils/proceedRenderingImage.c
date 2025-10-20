/* Extracted from B2DPlugin.c:9013 (function proceedRenderingImage). */

static sqInt proceedRenderingImage(void) {
  sqInt external;
  sqInt value;
  sqInt value1;
  sqInt valueSqInt;

  while (!((workBuffer[GWState]) == GEStateCompleted)) {
    if (doProfileStats) {
      geProfileTime = ioMicroMSecs();
    }
    external = findNextExternalEntryFromGET();
    if (doProfileStats) {
      /* incrementStat:by: */
      workBuffer[GWCountNextGETEntry] = ((workBuffer[GWCountNextGETEntry]) + 1);
      value = (ioMicroMSecs()) - geProfileTime;

      /* begin incrementStat:by: */
      workBuffer[GWTimeNextGETEntry] =
          ((workBuffer[GWTimeNextGETEntry]) + value);
    }
    if (engineStopped) {
      return workBuffer[GWState] = GEStateAddingFromGET;
    }
    if (external) {
      /* statePut: */
      workBuffer[GWState] = GEStateWaitingForEdge;

      /* begin stopBecauseOf: */
      /* stopReasonPut: */
      workBuffer[GWStopReason] = GErrorGETEntry;
      engineStopped = 1;
      return null;
    }

    /* aetStartPut: */
    workBuffer[GWAETStart] = 0;

    /* begin wbStackClear */
    /* wbTopPut: */
    workBuffer[GWBufferTop] = (workBuffer[GWSize]);

    /* clearSpanBufferPut: */
    workBuffer[GWClearSpanBuffer] = 1;
    if (doProfileStats) {
      geProfileTime = ioMicroMSecs();
    }
    if (((workBuffer[GWClearSpanBuffer]) != 0) &&
        (((workBuffer[GWCurrentY]) & (workBuffer[GWAAScanMask])) == 0)) {
      clearSpanBuffer();
    }

    /* clearSpanBufferPut: */
    workBuffer[GWClearSpanBuffer] = 0;
    external = findNextExternalFillFromAET();
    if (doProfileStats) {
      /* incrementStat:by: */
      workBuffer[GWCountNextFillEntry] =
          ((workBuffer[GWCountNextFillEntry]) + 1);
      valueSqInt = (ioMicroMSecs()) - geProfileTime;

      /* begin incrementStat:by: */
      workBuffer[GWTimeNextFillEntry] =
          ((workBuffer[GWTimeNextFillEntry]) + valueSqInt);
    }
    if (engineStopped) {
      return workBuffer[GWState] = GEStateScanningAET;
    }
    if (external) {
      /* statePut: */
      workBuffer[GWState] = GEStateWaitingForFill;

      /* begin stopBecauseOf: */
      /* stopReasonPut: */
      workBuffer[GWStopReason] = GErrorFillEntry;
      engineStopped = 1;
      return null;
    }

    /* begin wbStackClear */
    /* wbTopPut: */
    workBuffer[GWBufferTop] = (workBuffer[GWSize]);

    /* spanEndAAPut: */
    workBuffer[GWSpanEndAA] = 0;
    if (doProfileStats) {
      geProfileTime = ioMicroMSecs();
    }
    if (((workBuffer[GWCurrentY]) & (workBuffer[GWAAScanMask])) ==
        (workBuffer[GWAAScanMask])) {
      displaySpanBufferAt(workBuffer[GWCurrentY]);
      postDisplayAction();
    }
    if (doProfileStats) {
      /* incrementStat:by: */
      workBuffer[GWCountDisplaySpan] = ((workBuffer[GWCountDisplaySpan]) + 1);
      value1 = (ioMicroMSecs()) - geProfileTime;

      /* begin incrementStat:by: */
      workBuffer[GWTimeDisplaySpan] =
          ((workBuffer[GWTimeDisplaySpan]) + value1);
    }
    if (engineStopped) {
      return workBuffer[GWState] = GEStateBlitBuffer;
    }
    if ((workBuffer[GWState]) == GEStateCompleted) {
      return 0;
    }

    /* aetStartPut: */
    workBuffer[GWAETStart] = 0;

    /* currentYPut: */
    workBuffer[GWCurrentY] = ((workBuffer[GWCurrentY]) + 1);
    if (doProfileStats) {
      geProfileTime = ioMicroMSecs();
    }
    external = findNextExternalUpdateFromAET();
    if (doProfileStats) {
      /* incrementStat:by: */
      workBuffer[GWCountNextAETEntry] = ((workBuffer[GWCountNextAETEntry]) + 1);
      value1 = (ioMicroMSecs()) - geProfileTime;

      /* begin incrementStat:by: */
      workBuffer[GWTimeNextAETEntry] =
          ((workBuffer[GWTimeNextAETEntry]) + value1);
    }
    if (engineStopped) {
      return workBuffer[GWState] = GEStateUpdateEdges;
    }
    if (external) {
      /* statePut: */
      workBuffer[GWState] = GEStateWaitingChange;

      /* begin stopBecauseOf: */
      /* stopReasonPut: */
      workBuffer[GWStopReason] = GErrorAETEntry;
      engineStopped = 1;
      return null;
    }
  }
  return 0;
}