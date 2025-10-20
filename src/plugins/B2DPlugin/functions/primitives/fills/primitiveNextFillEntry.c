/* Extracted from B2DPlugin.c:8418 (function primitiveNextFillEntry). */

EXPORT(sqInt)
primitiveNextFillEntry(void) {
  sqInt failureCode;
  sqInt fillOop;
  sqInt hasFill;
  sqInt value;

  if (doProfileStats) {
    geProfileTime = ioMicroMSecs();
  }
  if (!((methodArgumentCount()) == 1)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFromrequiredState(stackValue(1),
                                                      GEStateScanningAET))) {
    return primitiveFailFor(failureCode);
  }

  /* Load span buffer for internal handling of fills */
  if ((failureCode =
           loadSpanBufferFrom(fetchPointerofObject(BESpanIndex, engine)))) {
    return primitiveFailFor(failureCode);
  }
  if (!(loadFormsFrom(fetchPointerofObject(BEFormsIndex, engine)))) {
    return primitiveFailFor(GEFFormLoadFailed);
  }

  /* Check if we have to clear the span buffer before proceeding */
  if (workBuffer[GWClearSpanBuffer]) {
    if (!((workBuffer[GWCurrentY]) & (workBuffer[GWAAScanMask]))) {
      clearSpanBuffer();
    }

    /* clearSpanBufferPut: */
    workBuffer[GWClearSpanBuffer] = 0;
  }
  fillOop = stackObjectValue(0);
  hasFill = findNextExternalFillFromAET();
  if (engineStopped) {
    return primitiveFailFor(GEFEngineStopped);
  }
  if (hasFill) {
    storeFillStateInto(fillOop);
  }
  if (failed()) {
    return primitiveFailFor(GEFWrongFill);
  }
  if (hasFill) {
    /* statePut: */
    workBuffer[GWState] = GEStateWaitingForFill;
  } else {
    /* begin wbStackClear */
    /* wbTopPut: */
    workBuffer[GWBufferTop] = (workBuffer[GWSize]);

    /* spanEndAAPut: */
    workBuffer[GWSpanEndAA] = 0;

    /* statePut: */
    workBuffer[GWState] = GEStateBlitBuffer;
  }
  workBuffer[GWObjUsed] = objUsed;
  pop(2);
  pushBool(!hasFill);
  if (doProfileStats) {
    /* incrementStat:by: */
    workBuffer[GWCountNextFillEntry] = ((workBuffer[GWCountNextFillEntry]) + 1);
    value = (ioMicroMSecs()) - geProfileTime;

    /* begin incrementStat:by: */
    workBuffer[GWTimeNextFillEntry] =
        ((workBuffer[GWTimeNextFillEntry]) + value);
  }
  return 0;
}