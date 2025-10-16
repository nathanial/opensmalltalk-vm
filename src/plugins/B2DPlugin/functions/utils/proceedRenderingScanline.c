/* Extracted from B2DPlugin.c:9163 (function proceedRenderingScanline). */

static sqInt proceedRenderingScanline(void) {
  sqInt external;
  int state;
  sqInt value;

  state = workBuffer[GWState];
  if (!state) {
    initializeGETProcessing();
    if (engineStopped) {
      return 0;
    }
    state = GEStateAddingFromGET;
  }
  if (state == GEStateAddingFromGET) {
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
    state = GEStateScanningAET;
  }
  if (state == GEStateScanningAET) {
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
      value = (ioMicroMSecs()) - geProfileTime;

      /* begin incrementStat:by: */
      workBuffer[GWTimeNextFillEntry] =
          ((workBuffer[GWTimeNextFillEntry]) + value);
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
    state = GEStateBlitBuffer;

    /* begin wbStackClear */
    /* wbTopPut: */
    workBuffer[GWBufferTop] = (workBuffer[GWSize]);

    /* spanEndAAPut: */
    workBuffer[GWSpanEndAA] = 0;
  }
  if (state == GEStateBlitBuffer) {
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
      value = (ioMicroMSecs()) - geProfileTime;

      /* begin incrementStat:by: */
      workBuffer[GWTimeDisplaySpan] = ((workBuffer[GWTimeDisplaySpan]) + value);
    }
    if (engineStopped) {
      return workBuffer[GWState] = GEStateBlitBuffer;
    }
    if ((workBuffer[GWState]) == GEStateCompleted) {
      return 0;
    }
    state = GEStateUpdateEdges;

    /* aetStartPut: */
    workBuffer[GWAETStart] = 0;

    /* currentYPut: */
    workBuffer[GWCurrentY] = ((workBuffer[GWCurrentY]) + 1);
  }
  if (state == GEStateUpdateEdges) {
    if (doProfileStats) {
      geProfileTime = ioMicroMSecs();
    }
    external = findNextExternalUpdateFromAET();
    if (doProfileStats) {
      /* incrementStat:by: */
      workBuffer[GWCountNextAETEntry] = ((workBuffer[GWCountNextAETEntry]) + 1);
      value = (ioMicroMSecs()) - geProfileTime;

      /* begin incrementStat:by: */
      workBuffer[GWTimeNextAETEntry] =
          ((workBuffer[GWTimeNextAETEntry]) + value);
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

    /* statePut: */
    workBuffer[GWState] = GEStateAddingFromGET;
  }
  return 0;
}